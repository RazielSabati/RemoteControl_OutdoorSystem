#ifndef HEATCONTROL_H
#define HEATCONTROL_H
#include <Arduino.h>

class HeatControl {
public:
    HeatControl(int pin);  // אתחול עם מספר הפין
    void turnOnHeat();
    void turnOffHeat();
private:
    int heatPin;
};

#endif
