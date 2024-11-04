#include "./communication/communication.h"
#include "./DisplayMenu/DisplayMenu.h"

ExternalCommunication externalComm;
DisplayMenu menu;

void setup() {
    Serial.begin(9600);
    while (!Serial && millis() < 5000);
    
    if (!externalComm.setupCommunication()) {
        Serial.println(F("Failed to initialize communication!"));
        while (1) {delay(1000);}
    }
    
    menu.setupScreen();
}

void loop() {
    externalComm.receiveMessage(menu);
    
    delay(10);// Add a small delay to prevent tight looping
} 