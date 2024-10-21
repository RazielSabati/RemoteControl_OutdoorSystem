#include "./communication/communication.h"

ExternalCommunication comms;

void setup() {
    Serial.begin(9600);
    while (!Serial);          // Wait until Serial is ready
    comms.setupCommunication();
    Serial.println("External system is ready to receive messages.");
}

void loop() {
    comms.receiveMessage();  // Receive a message and print if received
}