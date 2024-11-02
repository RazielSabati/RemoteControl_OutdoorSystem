#include "communication.h"

ExternalCommunication::ExternalCommunication() : actionHandler() {}

bool ExternalCommunication::setupCommunication() {
    LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    
    // Try to initialize LoRa with timeout
    unsigned long start = millis();
    while (!LoRa.begin(433E6)) {
        if (millis() - start > 5000) {
            Serial.println(F("LoRa initialization failed!"));
            return false;
        }
        delay(100);
    }
    
    // הגדרות למרחק מקסימלי
    LoRa.setSpreadingFactor(12); //שידור איטי יותר, אך החוסן והטווח גדלים
    LoRa.setSignalBandwidth(31.25E3); // רוחב פס מאוזן יותר
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(8);        // אורך preamble סטנדרטי
    LoRa.setTxPower(20);
    LoRa.enableCrc();
    
    Serial.println(F("LoRa initialized successfully."));
    return true;
}

void ExternalCommunication::receiveMessage(DisplayMenu& menu) {

    int packetSize = LoRa.parsePacket();

    if (packetSize == 0) 
            return;

    String receivedMessage = "";
    while (LoRa.available()) {
        char c = LoRa.read();  // קריאה של תו בודד
        receivedMessage += c;   // הוספת התו ל-String
    }

    if(receivedMessage.length() > 3){
        menu.displayReceivedMessage("Invalid packet size received");
        return;
    }

    int receivedNumber = receivedMessage.toInt();  // המרה למספר שלם

    Serial.print(F("Received number: "));
    Serial.println(receivedNumber);

    menu.displayReceivedMessage(String(receivedNumber) + " rssi:" + String(LoRa.packetRssi()) + " snr:" + String(LoRa.packetSnr()));

    // Extract menu type and action index
    int menuType = receivedNumber / 10 - 1;
    int actionIndex = receivedNumber % 10;
    
    if (menuType < 0 || menuType > 1 || actionIndex < 0 || actionIndex > 5) {
        Serial.println(F("Invalid command code received."));
        return;
    }
    
    // Execute action
    actionHandler.executeAction(menuType, actionIndex);
    if (!sendResponseWithRetry(receivedNumber, 5, menu)) 
        Serial.println(F("Failed to send acknowledgment after retries"));
            
}

bool ExternalCommunication::sendResponse(int response,DisplayMenu& menu) {
    String sendMessage = String(response);  // הפיכת המספר ל-String

    LoRa.beginPacket();
    LoRa.print(sendMessage);  // שליחה של ההודעה כמחרוזת
    bool success = LoRa.endPacket();
    
    if (success) {
        menu.displayReceivedMessage(sendMessage + ":Message sent successfully");
    } else {
        menu.displayReceivedMessage("Failed to send message");
    }

    delay(10);
    return success;
}

bool ExternalCommunication::sendResponseWithRetry(int response, int maxRetries,DisplayMenu& menu) {
    int ERROR = 0;

    for (int i = 0; i < maxRetries; i++) {
        if (sendResponse(response,menu)) {
        Serial.print(F("Retry "));
        Serial.print(i + 1);
        Serial.println(F(" of "));
        Serial.println(maxRetries);
        delay(20); // Increasing delay between retries
        }
        else 
        {
        ERROR++;
        Serial.print(F("cannot send response"));
        }
    }
    if (ERROR == maxRetries) {
        return false;
    }
    return true;
}

