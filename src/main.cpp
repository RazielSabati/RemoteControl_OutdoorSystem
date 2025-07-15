#include "./communication/communication.h"
#include "./DisplayMenu/DisplayMenu.h"
#include "./ActionHandler/ActionHandler.h"

ExternalCommunication externalComm;
DisplayMenu menu;
ActionHandler actionHandler;


void setup() {
    Serial.begin(9600);
    while (!Serial && millis() < 5000);
    
    if (!externalComm.setupCommunication()) {
        Serial.println(F("Failed to initialize communication!"));
        while (1) {delay(1000);}
    }
    
    menu.setupScreen();
    analogReadResolution(12);
}

void loop() {
    externalComm.receiveMessage(menu);
    

    actionHandler.readBatteryVoltage();

    delay(10);// Add a small delay to prevent tight looping

} 