#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>                 // Library for SPI communication required for the LoRa module
#include <LoRa.h>                // LoRa library enabling communication with the LoRa module
#include "../ActionHandler/ActionHandler.h" 
#include "../DisplayMenu/DisplayMenu.h"
#include <AESLib.h>

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

        // מפתח ההצפנה (32 בתים = 256 ביט)
 // מפתח AES באורך 16 בתים (128 ביט)
    byte key[16] = {0x2b, 0x7e, 0x15, 0x16, 0xae, 0x2a, 0xd4, 0xa6, 0x8a, 0xf7, 0x97, 0x75, 0x40, 0x8e, 0x6a, 0x12};



    uint8_t buffer[32];

    uint32_t rollingCode; // המידע להצפנה

    byte iv[16]; // וקטור אתחול באורך 96 ביטים (12 בתים עבור ChaCha20)
    byte iv_for_en[16]; 

    uint8_t plaintext[16]; // ההודעה המקורית
    uint8_t encrypted[16]; // ההודעה המוצפנת
    uint8_t decrypted[16]; // ההודעה המפוענחת
    
    AESLib aesLib;

    int randomNumber;

public:
    ExternalCommunication();                
    bool setupCommunication();              
    void receiveMessage(DisplayMenu& menu); 
    void generateRandomIV();

};

#endif // COMMUNICATION_H