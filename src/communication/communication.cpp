#include "communication.h"

#define ss 5
#define rst 14
#define dio0 2

ExternalCommunication::ExternalCommunication() {}

void ExternalCommunication::setupCommunication() {
        
    LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa failed to start!");
        while (1);  // Stop the code in case of initialization failure
    }
    Serial.println("LoRa initialized successfully.");
}

void ExternalCommunication::receiveMessage() {
    int packetSize = LoRa.parsePacket();
    
    if (packetSize) {
        String message = "";
        while (LoRa.available()) {
            message += (char)LoRa.read();
        }

        Serial.println("Message received: " + message);
        
        delay(1000);  // Wait for 3 seconds
        sendResponse("ACK");
    }

    
}

void ExternalCommunication::sendResponse(String response) {
    Serial.print("Sending message: ");
    Serial.println(response);

    LoRa.beginPacket();       
    LoRa.print(response);     
    LoRa.endPacket();         
    delay(1000);              

}