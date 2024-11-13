#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>                 // Library for SPI communication required for the LoRa module
#include <LoRa.h>                // LoRa library enabling communication with the LoRa module
#include "../ActionHandler/ActionHandler.h" 
#include "../DisplayMenu/DisplayMenu.h"

// Task function that will run on the other core
void sendResponseTask(void* parameters);

// Structure to hold task parameters
struct TaskParams {
    uint16_t message;         
    DisplayMenu* menu;        
};

// Class to handle external communication using LoRa
class ExternalCommunication {
private:
    static const uint8_t LORA_SS_PIN = 5;   // LoRa module SS pin
    static const uint8_t LORA_RST_PIN = 14; // LoRa module reset pin
    static const uint8_t LORA_DIO0_PIN = 2; // LoRa module DIO0 pin

    ActionHandler actionHandler;            // Object to handle actions

public:
    ExternalCommunication();                
    bool setupCommunication();              
    void receiveMessage(DisplayMenu& menu); 
    uint8_t lfsrScramble(uint8_t unscrambled);
    uint8_t lfsrUnscramble(uint8_t scrambled); 
};

#endif // COMMUNICATION_H