#include "HeatControl.h"

bool heatingActive = false;  // Global variable to track the heating status

// Constructor for the HeatControl class
HeatControl::HeatControl(int pin, int sensorPin, int targetTemperature)
    : heatPin(pin), sensorPin(sensorPin), targetTemperature(targetTemperature), dht(sensorPin, DHT22) {
    pinMode(heatPin, OUTPUT);  // Set the heating pin as output
    dht.begin();  // Initialize the DHT sensor
}

// Function to turn on the heating
bool HeatControl::turnOnHeat() {
    if (heatingActive) {
        Serial.println("Heating is already active.");
        return false;  // Return false if heating is already active
    }

    // Create a task to control the heating loop
    xTaskCreate([](void* pvParameters) {
        HeatControl* hc = (HeatControl*)pvParameters;
        hc->heatControlLoop();
        vTaskDelete(NULL);  // Delete the task when done
    }, "HeatControlTask", 4096, this, 1, &heatingTaskHandle);

    delay(100);  // Short delay to ensure the task starts
    return heatingActive;  // Return the status of heating
}

// Function to turn off the heating
bool HeatControl::turnOffHeat() {
    if (!heatingActive) {
        Serial.println("Heating is already off.");
        return false;  // Return false if heating is already off
    }

    // Delete the heating task if it exists
    if (heatingTaskHandle != NULL) {
        vTaskDelete(heatingTaskHandle);
        heatingTaskHandle = NULL;
    }

    analogWrite(heatPin, 0);  // Turn off the heating pin
    heatingActive = false;  // Update the heating status
    Serial.println("Heating off.");
    return true;  // Return true indicating the heating was turned off
}

// Function to control the heating loop
void HeatControl::heatControlLoop() {
    heatingActive = true;  // Set the heating status to active
    int pwmValue = 50;  // Initial PWM value for efficient operation (low PWM)

    while (heatingActive) {
        float temp = dht.readTemperature();  // Read the temperature from the sensor

        if (isnan(temp)) {
            Serial.println("Failed to read temperature!");
            return;  // Exit the loop if reading the temperature fails
        }

        Serial.print("Current Temperature: ");
        Serial.print(temp);
        Serial.println("°C");

        if (temp < targetTemperature) {
            analogWrite(heatPin, pwmValue);  // Set the PWM value to the heating pin
            Serial.print("PWM Value: ");
            Serial.println(pwmValue);

            if (temp < targetTemperature - 1) {
                pwmValue = min(pwmValue + 5, 150);  // Gradually increase the PWM value (up to 150)
            }
        } else {
            analogWrite(heatPin, 0);  // Turn off the heating pin
            Serial.println("Target temperature reached, heating off.");
            heatingActive = false;  // Update the heating status
        }

        delay(10000);  // Wait for 10 seconds before reading the temperature again
    }
}