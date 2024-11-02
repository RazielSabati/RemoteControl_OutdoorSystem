#include "LightControl.h"

LightControl::LightControl() {
    pinMode(RELAY_PIN, OUTPUT);  // הגדרת הפין כיציאה
    digitalWrite(RELAY_PIN, HIGH); // כיבוי ברירת מחדל (LOW) - LED כבוי
    
}

void LightControl::turnOnLED() {
    digitalWrite(RELAY_PIN, LOW); // הדלקת ה-LED
    Serial.println("LED turned ON");
    }

void LightControl::turnOffLED() {
    digitalWrite(RELAY_PIN, HIGH); // כיבוי ה-LED
    Serial.println("LED turned OFF");
}
