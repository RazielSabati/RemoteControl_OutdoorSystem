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
    
    LoRa.setSpreadingFactor(12); 
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(8);
    LoRa.setTxPower(20);
    // LoRa.enableCrc();
    
    Serial.println(F("LoRa initialized successfully."));
    return true;
}

// Task function to send a response, runs on a different core
void sendResponseTask(void* parameters) {
    TaskParams* taskParams = (TaskParams*)parameters;
    uint8_t response = taskParams->message;
    DisplayMenu* menu = taskParams->menu;
    
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

// Function to receive a message and update the display menu
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
        menu.displayTopScreen("Invalid command code");
        return;
    }

    int calculatedChecksum = ((menuType << 3) | actionIndex) ^ ERROR_CHECK_MASK;
    calculatedChecksum &= 0x0F;

    if (calculatedChecksum != receivedChecksum) {
        Serial.println(F("Checksum does not match."));
        menu.displayTopScreen("Checksum error");
        return;
    }

    String actionMessage = "Executing action...                       ";

    menu.updateMiddleScreen(actionMessage + String(menu.getData(menuType, actionIndex)));

    if (actionHandler.executeAction(menuType, actionIndex)) {
        actionMessage = String(menu.getData(menuType, actionIndex)) + " done!";
    } else {
        actionMessage = "Failed to execute action!";
    }

    menu.updateBottomScreen(actionMessage);

    // Create dynamic struct for task parameters
    TaskParams* taskParams = new TaskParams;
    taskParams->message = receivedByte; // Use receivedByte directly
    taskParams->menu = &menu; // Pass the address of the menu

    // Create task on core 0
    TaskHandle_t sendingTaskHandle = NULL;
    BaseType_t result = xTaskCreatePinnedToCore(
        sendResponseTask,      // Task function
        "SendResponse",        // Task name
        2048,                  // Stack size
        taskParams,            // Parameters (byte to send)
        1,                     // Priority
        &sendingTaskHandle,    // Task handle
        0                      // Core ID (0)
    );
    
    if (result != pdPASS) {
        Serial.println(F("Failed to create sending task"));
        menu.displayTopScreen("Failed to start sending task");
        delete taskParams;
    } else {
        menu.displayTopScreen("Started sending response");
    }
}