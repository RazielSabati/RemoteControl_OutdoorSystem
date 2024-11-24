#include "communication.h"

// Constructor of the ExternalCommunication class
ExternalCommunication::ExternalCommunication() : actionHandler() {
    memset(iv, 0, sizeof(iv));
    memset(plaintext, 0, sizeof(plaintext));
    memset(encrypted, 0, sizeof(encrypted));
    memset(decrypted, 0, sizeof(decrypted));
    rollingCode = 0x00;
}

// Function to set up LoRa communication
bool ExternalCommunication::setupCommunication() {
    LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    
    unsigned long start = millis();
    while (!LoRa.begin(433E6)) {
        if (millis() - start > 5000) {
            Serial.println(F("LoRa initialization failed!"));
            return false;
        }
        delay(100);
    }
    
    // LoRa initialization
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setSpreadingFactor(12);
    LoRa.setCodingRate4(8);
    LoRa.setTxPower(20);

    Serial.println(F("LoRa initialized successfully."));
    return true;
}



// Function to receive a message and update the display menu
void ExternalCommunication::receiveMessage(DisplayMenu& menu) {
    int packetSize = LoRa.parsePacket();

    if (packetSize != 32) { // ה-IV הוא 12 בתים וההודעה המוצפנת היא 16 בתים
        return;
    }

    // קריאת הנתונים מהחבילה
    LoRa.readBytes(buffer, 32);

    // חילוץ ה-IV (12 בתים ראשונים)
    memcpy(iv, buffer, 16);

    // חילוץ ההודעה המוצפנת (16 בתים הבאים)
    memcpy(encrypted, buffer + 16, 16);

    aesLib.decrypt(encrypted, 16, decrypted, key, 128, iv);

    // בדיקת ה-checksum
    uint8_t checksum = decrypted[15];
    if (checksum != (decrypted[0] ^ 0xF0)) {
        Serial.println(F("Checksum error."));
        return;
    }

    // חילוץ ה-rolling code (4 בתים)
    uint32_t rollingCodeReceived = 
        (decrypted[1] << 24) | 
        (decrypted[2] << 16) | 
        (decrypted[3] << 8) | 
        decrypted[4];

 

    if(rollingCodeReceived <= rollingCode  || rollingCodeReceived > rollingCode + 50) {
        if(rollingCodeReceived == 0) {
            rollingCodeReceived = 0;
        } 
        else {
            Serial.println(F("Received message with invalid rolling code."));
            return;
        }
    }

    // חילוץ המידע מההודעה
    int menuType = (decrypted[0] >> 7) & 0x01;   // ביט המשמעותי ביותר
    int actionIndex = (decrypted[0] >> 4) & 0x07; // 3 ביטים הבאים

    // אימות הנתונים
    if (menuType < 0 || menuType > 1 || actionIndex < 0 || actionIndex > 7) {
        Serial.println(F("Invalid command code received."));
        menu.displayTopScreen("Invalid command code");
        return;
    }

    // עדכון התצוגה
    String actionMessage = "Executing action... ";
    menu.updateMiddleScreen(actionMessage + String(menu.getData(menuType, actionIndex)));

    if (actionHandler.executeAction(menuType, actionIndex)) {
        actionMessage = String(menu.getData(menuType, actionIndex)) + " done!";
    } else {
        actionMessage = "Failed to execute action!";
    }

    menu.updateBottomScreen(actionMessage);


    // עדכון ה-rolling code
    rollingCode = rollingCodeReceived + 1;


    // Create a response message

    // Put the rolling code in the next 4 bytes
    decrypted[1] =  (rollingCode >> 24) & 0xFF; 
    decrypted[2] = (rollingCode >> 16) & 0xFF;     
    decrypted[3] = (rollingCode >> 8) & 0xFF;     
    decrypted[4] = rollingCode & 0xFF;     

    generateRandomIV(); // Generate a random IV for encryption

    // Encrypt the response message using AES with the new IV
    aesLib.encrypt(decrypted, 16, encrypted, key, 128, iv_for_en);

    // Send the response message
    LoRa.beginPacket();
    LoRa.write(iv, 16);
    LoRa.write(encrypted, 16);
    bool success = LoRa.endPacket();
    if (success) {
        menu.displayTopScreen("Sending response done");
        Serial.print(F("Sent message: "));
    }
    else {
        menu.displayTopScreen("Failed to send response");
        Serial.println(F("Failed to send message"));
    }    

}


void ExternalCommunication::generateRandomIV() {
    for (int i = 0; i < 16; i++) {
        iv[i] = random(0, 256);
    }
    // Copy the IV to iv_for_en
    memcpy(iv_for_en, iv, 16);
}