#include "communication.h"

// Constructor of the ExternalCommunication class
ExternalCommunication::ExternalCommunication() : actionHandler() {}

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

// Task function to send a response, runs on a different core
void sendResponseTask(void* parameters) {
    TaskParams* taskParams = (TaskParams*)parameters;
    uint16_t response = taskParams->message;
    DisplayMenu* menu = taskParams->menu;
    
    unsigned long startTime = millis();
    int successCount = 0;
    
    // Try sending twice with 15 seconds timeout
    for(int i = 0; i < 2 && (millis() - startTime) < 15000; i++) {
        String sendMessage = String(response);
        
        LoRa.beginPacket();
        LoRa.write((uint8_t*)&response, sizeof(response));    
        bool success = LoRa.endPacket();
        
        if(success) {
            successCount++;
            Serial.print(F("Message sent successfully attempt "));
            Serial.println(i + 1);
        } else {
            Serial.println(F("Failed to send message"));
        }
        
        delay(40); // Short delay between attempts
    }
    
    if(successCount == 0) {
        Serial.println(F("Failed to send any messages within timeout"));
    } else {
        menu->displayTopScreen("Finish sending response");
    }
    
    delete taskParams; // Clean up
    vTaskDelete(NULL); // Delete task
}

// Function to scramble a byte using LFSR
uint8_t ExternalCommunication::lfsrScramble(uint8_t unscrambled) {
    uint8_t key = 0x3A;  // Fixed key for scrambling
    unscrambled ^= key;

    // Swap between the second and third bits
    bool bit1 = (unscrambled >> 1) & 0x01;
    bool bit2 = (unscrambled >> 2) & 0x01;
    unscrambled ^= (-bit1 ^ unscrambled) & (1 << 2);
    unscrambled ^= (-bit2 ^ unscrambled) & (1 << 1);

    // Swap between the fourth and eighth bits
    bool bit3 = (unscrambled >> 3) & 0x01;
    bool bit7 = (unscrambled >> 7) & 0x01;
    unscrambled ^= (-bit3 ^ unscrambled) & (1 << 7);
    unscrambled ^= (-bit7 ^ unscrambled) & (1 << 3);

    // Swap between the fifth and seventh bits
    bool bit4 = (unscrambled >> 4) & 0x01;
    bool bit6 = (unscrambled >> 6) & 0x01;
    unscrambled ^= (-bit4 ^ unscrambled) & (1 << 6);
    unscrambled ^= (-bit6 ^ unscrambled) & (1 << 4);

    return unscrambled;
}

// Function to unscramble a byte using LFSR
uint8_t ExternalCommunication::lfsrUnscramble(uint8_t scrambled) {
    uint8_t key = 0x3A;  // Same fixed key for XOR

    // Swap between the fifth and seventh bits
    bool bit4 = (scrambled >> 4) & 0x01;
    bool bit6 = (scrambled >> 6) & 0x01;
    scrambled ^= (-bit4 ^ scrambled) & (1 << 6);
    scrambled ^= (-bit6 ^ scrambled) & (1 << 4);

    // Swap between the fourth and eighth bits
    bool bit3 = (scrambled >> 3) & 0x01;
    bool bit7 = (scrambled >> 7) & 0x01;
    scrambled ^= (-bit3 ^ scrambled) & (1 << 7);
    scrambled ^= (-bit7 ^ scrambled) & (1 << 3);

    // Swap between the second and third bits
    bool bit1 = (scrambled >> 1) & 0x01;
    bool bit2 = (scrambled >> 2) & 0x01;
    scrambled ^= (-bit1 ^ scrambled) & (1 << 2);
    scrambled ^= (-bit2 ^ scrambled) & (1 << 1);

    // XOR with the same fixed key to return to the original state
    scrambled ^= key;

    return scrambled;
}

// Function to receive a message and update the display menu
void ExternalCommunication::receiveMessage(DisplayMenu& menu) {
    int packetSize = LoRa.parsePacket();

    if (packetSize != 2) {
        return;
    }
    uint8_t buffer[2];
    LoRa.readBytes(buffer, 2);

    // Extract the message from the received bytes
    uint8_t receivedMessage1 = buffer[0];
    uint8_t receivedMessage2 = buffer[1];

    Serial.print(F("Received message 1: 0b"));  
    Serial.println(receivedMessage1, BIN);
    Serial.print(F("Received message 2: 0b"));
    Serial.println(receivedMessage2, BIN);

    // Check if both received messages are identical
    if (receivedMessage1 != receivedMessage2) {
        Serial.println(F("Message mismatch error."));
        return;
    }

    uint8_t scrambledMessage = receivedMessage1;

    Serial.print(F("Received scrambled message: 0b"));
    Serial.println(scrambledMessage, BIN);

    // Unscramble the received message
    uint8_t decryptedMessage = lfsrUnscramble(scrambledMessage); 

    Serial.print(F("Decrypted message: 0b"));
    Serial.println(decryptedMessage, BIN);

    // Extract data from the decrypted message
    int menuType = (decryptedMessage >> 7) & 0x01;       // Most significant bit
    int actionIndex = (decryptedMessage >> 4) & 0x07;    // Next 3 bits
    int receivedRandomNumber = decryptedMessage & 0x0F;  // Least significant 4 bits

    Serial.print(F("Menu type: "));
    Serial.println(menuType);
    Serial.print(F("Action index: "));
    Serial.println(actionIndex);
    Serial.print(F("Received random number: "));
    Serial.println(receivedRandomNumber);

    // Validate the data
    if (menuType < 0 || menuType > 1 || actionIndex < 0 || actionIndex > 5) {
        Serial.println(F("Invalid command code received."));
        menu.displayTopScreen("Invalid command code");
        return;
    }

    // Update display according to the action
    String actionMessage = "Executing action... ";
    menu.updateMiddleScreen(actionMessage + String(menu.getData(menuType, actionIndex)));

    if (actionHandler.executeAction(menuType, actionIndex)) {
        actionMessage = String(menu.getData(menuType, actionIndex)) + " done!";
    } else {
        actionMessage = "Failed to execute action!";
    }

    menu.updateBottomScreen(actionMessage);

    // Create a response message
    int randomNumber = random(0, 16);
    uint8_t response = (menuType & 0x01) << 7;           // Insert menuType in the most significant bit
    response |= (actionIndex & 0x07) << 4;               // Insert actionIndex in the next 3 bits
    response |= (randomNumber & 0x0F);                   // Insert randomNumber in the least significant 4 bits

    // Scramble the response message
    uint8_t scrambledResponse = lfsrScramble(response);

    Serial.print(F("Response message (scrambled): 0b"));
    Serial.println(scrambledResponse, BIN);

    // Send the response message twice
    LoRa.beginPacket();
    LoRa.write(scrambledResponse);
    LoRa.write(scrambledResponse);
    LoRa.endPacket();

    Serial.println(F("Response sent successfully."));
}