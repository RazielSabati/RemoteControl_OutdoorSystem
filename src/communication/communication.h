#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>      // Library for SPI communication required for the LoRa module
#include <LoRa.h>     // LoRa library enabling communication with the LoRa module

class ExternalCommunication {
public:
    ExternalCommunication();  // Constructor
    void setupCommunication();  // Initialize LoRa communication
    void receiveMessage();  // Receive a message from the home system
    void sendResponse(String response);  // Send a response to the home system
};

#endif