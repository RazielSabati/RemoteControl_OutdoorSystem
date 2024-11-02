#include "SoundControl.h"


SoundControl::SoundControl(int pin) : soundPin(pin) {
    pinMode(soundPin, OUTPUT);
}

void SoundControl::turnOnVoice() {
    digitalWrite(soundPin, HIGH);
    Serial.println("Sound turned on.");
}

void SoundControl::turnOffVoice() {
    digitalWrite(soundPin, LOW);
    Serial.println("Sound turned off.");
}
