#include "communication.h"

#define ss 5
#define rst 14
#define dio0 2
#define LED_BUILTIN 2 // Define the built-in LED pin on the ESP32 (built-in indicator LED)

ExternalCommunication::ExternalCommunication() {}

void ExternalCommunication::setupCommunication() {
    
    pinMode(LED_BUILTIN, OUTPUT);  // Set the built-in LED pin as output
    

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
        
        // Turn on the built-in LED for one second   
        digitalWrite(LED_BUILTIN, HIGH);   // Turn on the built-in LED
        delay(3000);  // Wait for 3 seconds
        digitalWrite(LED_BUILTIN, LOW);    // Turn off the built-in LED

    }
}