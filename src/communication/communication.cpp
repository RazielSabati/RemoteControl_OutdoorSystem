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

        // Check if we received exactly one byte (8 bits)
    if (packetSize != 1) {
        // Serial.println(F("Invalid packet size received."));
        return;  // Exit the function if packet size is not exactly 1 byte
    }

    uint8_t receivedByte = LoRa.read();  // Read the single byte message

    Serial.print(F("Received byte: 0b"));
    for (int i = 7; i >= 0; i--) {
        Serial.print((receivedByte >> i) & 0x01);
    }
    Serial.println();

    // Extract menu type, action index, and checksum
    int menuType = (receivedByte >> 7) & 0x01;      // Extract the first bit (menuType)
    int actionIndex = (receivedByte >> 4) & 0x07;   // Extract the next 3 bits (actionIndex)
    int receivedChecksum = receivedByte & 0x0F;     // Extract the last 4 bits for checksum

    // Validate menuType and actionIndex range
    if (menuType < 0 || menuType > 1 || actionIndex < 0 || actionIndex > 5) {
        Serial.println(F("Invalid command code received."));
        return;
    }

    // Calculate checksum
    int calculatedChecksum = ((menuType << 3) | actionIndex) ^ ERROR_CHECK_MASK; // Match the encoding in sendMessage
    calculatedChecksum &= 0x0F;  // Keep only the least significant 4 bits

    // Check if calculated checksum matches the received checksum
    if (calculatedChecksum != receivedChecksum) {
        Serial.println(F("Checksum does not match."));
        return;
    }

    // Execute action
    actionHandler.executeAction(menuType, actionIndex);
    if (!sendResponseWithRetry(receivedByte, 5, menu)) 
        Serial.println(F("Failed to send acknowledgment after retries"));
}


bool ExternalCommunication::sendResponse(uint8_t response,DisplayMenu& menu) {
    String sendMessage = String(response);  // הפיכת המספר ל-String

    LoRa.beginPacket();
    LoRa.write(response);
    bool success = LoRa.endPacket();
    
    if (success) {
        menu.displayReceivedMessage(sendMessage + ":Message sent successfully");
    } else {
        menu.displayReceivedMessage("Failed to send message");
    }

    delay(10);
    return success;
}

bool ExternalCommunication::sendResponseWithRetry(uint8_t response, int maxRetries,DisplayMenu& menu) {
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

