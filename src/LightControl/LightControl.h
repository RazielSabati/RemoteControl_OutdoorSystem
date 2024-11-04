#ifndef LIGHTCONTROL_H
#define LIGHTCONTROL_H

#include <Arduino.h>
#define RELAY_PIN 22  // Define the relay pin

class LightControl {
public:
    LightControl();      // Constructor
    bool turnOnLED();    // Function to turn on the LED, returns a boolean
    bool turnOffLED();   // Function to turn off the LED, returns a boolean
private:
    int lightPin;        // Pin number for the light control
};

#endif