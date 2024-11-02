#include "HeatControl.h"

HeatControl::HeatControl(int pin) : heatPin(pin) {
    pinMode(heatPin, OUTPUT);
}

void HeatControl::turnOnHeat() {
    digitalWrite(heatPin, HIGH);
    Serial.println("Heat turned on.");
}

void HeatControl::turnOffHeat() {
    digitalWrite(heatPin, LOW);
    Serial.println("Heat turned off.");
}
