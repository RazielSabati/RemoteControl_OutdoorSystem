#include "communication.h"

ExternalCommunication::ExternalCommunication() : actionHandler() {}

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
    
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(31.25E3);
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(8);
    LoRa.setTxPower(20);
    LoRa.enableCrc();
    
    Serial.println(F("LoRa initialized successfully."));
    return true;
}

void sendResponseTask(void* parameters) {
    uint8_t* messageParams = (uint8_t*)parameters;
    uint8_t response = *messageParams;
    
    unsigned long startTime = millis();
    int successCount = 0;
    
    // Try sending twice with 15 seconds timeout
    for(int i = 0; i < 2 && (millis() - startTime) < 15000; i++) {
        String sendMessage = String(response);
        
        LoRa.beginPacket();
        LoRa.write(response);
        bool success = LoRa.endPacket();
        
        if(success) {
            successCount++;
            Serial.print(F("Message sent successfully attempt "));
            Serial.println(i + 1);
        } else {
            Serial.println(F("Failed to send message"));
        }
        
        delay(20); // Short delay between attempts
    }
    
    if(successCount == 0) {
        Serial.println(F("Failed to send any messages within timeout"));
    }
    
    delete messageParams; // Clean up
    vTaskDelete(NULL);    // Delete task
}

void ExternalCommunication::receiveMessage(DisplayMenu& menu) {
    int packetSize = LoRa.parsePacket();

    if (packetSize != 1) {
        return;
    }

    uint8_t receivedByte = LoRa.read();

    Serial.print(F("Received byte: 0b"));
    for (int i = 7; i >= 0; i--) {
        Serial.print((receivedByte >> i) & 0x01);
    }
    Serial.println();

    int menuType = (receivedByte >> 7) & 0x01;
    int actionIndex = (receivedByte >> 4) & 0x07;
    int receivedChecksum = receivedByte & 0x0F;

    if (menuType < 0 || menuType > 1 || actionIndex < 0 || actionIndex > 5) {
        Serial.println(F("Invalid command code received."));
        return;
    }

    int calculatedChecksum = ((menuType << 3) | actionIndex) ^ ERROR_CHECK_MASK;
    calculatedChecksum &= 0x0F;

    if (calculatedChecksum != receivedChecksum) {
        Serial.println(F("Checksum does not match."));
        return;
    }

    menu.displayReceivedMessage(  menu.getData(menuType, actionIndex)  );
    actionHandler.executeAction(menuType, actionIndex);

    
    // Create dynamic byte for task parameters
    uint8_t* messageParams = new uint8_t(receivedByte);
    
    // Create task on core 0
    TaskHandle_t sendingTaskHandle = NULL;
    BaseType_t result = xTaskCreatePinnedToCore(
        sendResponseTask,      // Task function
        "SendResponse",        // Task name
        2048,                  // Stack size
        messageParams,         // Parameters (byte to send)
        1,                     // Priority
        &sendingTaskHandle,    // Task handle
        0                      // Core ID (0)
    );
    
    if(result != pdPASS) {
        Serial.println(F("Failed to create sending task"));
        menu.displayReceivedMessage("Failed to start sending task");
        delete messageParams;
    } else {
        menu.displayReceivedMessage("Started sending response");
    }
}