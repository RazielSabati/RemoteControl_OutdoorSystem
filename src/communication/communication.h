#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>                 // Library for SPI communication required for the LoRa module
#include <LoRa.h>                // LoRa library enabling communication with the LoRa module
#include "freertos/FreeRTOS.h"  // FreeRTOS library for task handling
#include "freertos/task.h" // FreeRTOS library for task handling
#include "../ActionHandler/ActionHandler.h" 
#include "../DisplayMenu/DisplayMenu.h"

// Task function that will run on the other core
void sendResponseTask(void* parameters);

// Structure to hold task parameters
struct TaskParams {
    uint8_t message;         
    DisplayMenu* menu;        
};

// Class to handle external communication using LoRa
class ExternalCommunication {
private:
    static const uint8_t LORA_SS_PIN = 5;   // LoRa module SS pin
    static const uint8_t LORA_RST_PIN = 14; // LoRa module reset pin
    static const uint8_t LORA_DIO0_PIN = 2; // LoRa module DIO0 pin

    ActionHandler actionHandler;            // Object to handle actions
    const uint8_t ERROR_CHECK_MASK = 0x0F;  // Mask for error checking

public:
    ExternalCommunication();                
    bool setupCommunication();              
    void receiveMessage(DisplayMenu& menu); 
};

#endif // COMMUNICATION_H