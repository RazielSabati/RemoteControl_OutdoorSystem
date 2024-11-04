#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>                 // Library for SPI communication required for the LoRa module
#include <LoRa.h>                // LoRa library enabling communication with the LoRa module
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../ActionHandler/ActionHandler.h"
#include "../DisplayMenu/DisplayMenu.h"

// Task function that will run on the other core
void sendResponseTask(void* parameters);

class ExternalCommunication {
private:
    ActionHandler actionHandler;
    static const uint8_t LORA_SS_PIN = 5;
    static const uint8_t LORA_RST_PIN = 14;
    static const uint8_t LORA_DIO0_PIN = 2;
    const uint8_t ERROR_CHECK_MASK = 0x0F;

public:
    ExternalCommunication(); 
    bool setupCommunication();
    void receiveMessage(DisplayMenu& menu);
};

#endif // COMMUNICATION_H