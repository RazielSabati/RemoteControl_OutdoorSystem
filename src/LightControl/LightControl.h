#ifndef LIGHTCONTROL_H
#define LIGHTCONTROL_H

#include <Arduino.h>
#define RELAY_PIN 22  // הגדרת הפין של הממסר

class LightControl {
public:
    LightControl();
    void turnOnLED();
    void turnOffLED();
private:
    int lightPin;
};

#endif
