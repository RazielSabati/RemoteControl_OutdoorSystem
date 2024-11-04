#include "LightControl.h"

LightControl::LightControl() {
    pinMode(RELAY_PIN, OUTPUT);  // Set the relay pin as an output
    digitalWrite(RELAY_PIN, HIGH); // Default to off (HIGH) - LED off
}

bool LightControl::turnOnLED() {
    digitalWrite(RELAY_PIN, LOW); // Turn on the LED
    
    // Check if the LED is on as expected
    if (digitalRead(RELAY_PIN) == LOW) { 
        Serial.println("LED turned ON");
        return true; // Return true if the operation succeeded
    } else {
        Serial.println("Failed to turn ON LED"); 
        return false; // Return false if the operation failed
    }
}

bool LightControl::turnOffLED() {
    digitalWrite(RELAY_PIN, HIGH); // Turn off the LED
    
    // Check if the LED is off as expected
    if (digitalRead(RELAY_PIN) == HIGH) { 
        Serial.println("LED turned OFF");
        return true; // Return true if the operation succeeded
    } else {
        Serial.println("Failed to turn OFF LED");
        return false; // Return false if the operation failed
    }
}