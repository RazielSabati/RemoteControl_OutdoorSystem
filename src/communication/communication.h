#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SPI.h>                 // Library for SPI communication required for the LoRa module
#include <LoRa.h>                // LoRa library enabling communication with the LoRa module
#include "../ActionHandler/ActionHandler.h" 
#include "../DisplayMenu/DisplayMenu.h"
#include <AESLib.h>

// Class to handle external communication using LoRa
class ExternalCommunication {
private:
    static const uint8_t LORA_SS_PIN = 5;   // LoRa module SS pin
    static const uint8_t LORA_RST_PIN = 21; // LoRa module reset pin
    static const uint8_t LORA_DIO0_PIN = 2; // LoRa module DIO0 pin

    ActionHandler actionHandler;            // Object to handle actions

    // AES encryption key (16 bytes = 128 bits)
    byte key[16] = {0x2b, 0x7e, 0x15, 0x16, 0xae, 0x2a, 0xd4, 0xa6, 0x8a, 0xf7, 0x97, 0x75, 0x40, 0x8e, 0x6a, 0x12};

    uint8_t buffer[32]; // Buffer to hold incoming data

    uint32_t rollingCode; // Rolling code for encryption

    byte iv[16]; // Initialization vector for encryption
    byte iv_for_en[16]; // Initialization vector for encryption response

    uint8_t plaintext[16]; // Original message
    uint8_t encrypted[16]; // Encrypted message
    uint8_t decrypted[16]; // Decrypted message
    
    AESLib aesLib; // AES library instance

    int randomNumber; // Random number for encryption

public:
    ExternalCommunication();                // Constructor
    bool setupCommunication();              // Function to set up LoRa communication
    void receiveMessage(DisplayMenu& menu); // Function to receive a message and update the display menu
    void generateRandomIV();                // Function to generate a random initialization vector

};

#endif // COMMUNICATION_H