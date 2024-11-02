#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>      // Library for SPI communication required for the LoRa module
#include <LoRa.h>     // LoRa library enabling communication with the LoRa module
#include <Arduino.h>
#include "../ActionHandler/ActionHandler.h"
#include "../DisplayMenu/DisplayMenu.h"
#define ANTENNA_GAIN 12.0


class ExternalCommunication {
private:
    ActionHandler actionHandler;
    static const uint8_t LORA_SS_PIN = 5;
    static const uint8_t LORA_RST_PIN = 14;
    static const uint8_t LORA_DIO0_PIN = 2;
    bool sendResponseWithRetry(int response, int maxRetries ,DisplayMenu& menu);
    DisplayMenu menu;

public:
    ExternalCommunication(); 
    bool setupCommunication();
    void receiveMessage(DisplayMenu& menu);
    bool sendResponse(int response,DisplayMenu& menu);
};

#endif