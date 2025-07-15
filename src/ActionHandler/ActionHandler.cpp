#include "ActionHandler.h"
#include <Arduino.h>
float samples[MAX_SAMPLES] = {0};


// Constructor
ActionHandler::ActionHandler() 
    : lightControl(), heatControl(26, 13), soundControl(16, Serial1) {  // Create instances with default values
    
    // Define the actions matrix with functions that return bool
    actionsMatrix[0][0] = [this]() { return executeTestScenario(); };
    actionsMatrix[0][1] = [this]() { return readBatteryPercentage(); };
    actionsMatrix[0][2] = [this]() { return turnOffSystem(); };
    actionsMatrix[0][3] = [this]() { return executeScenario1(); };
    actionsMatrix[0][4] = [this]() { return executeScenario2(); };
    actionsMatrix[0][5] = nullptr; // Not active

    actionsMatrix[1][0] = [this]() { return lightControl.turnOnLED(); };
    actionsMatrix[1][1] = [this]() { return lightControl.turnOffLED(); };
    actionsMatrix[1][2] = [this]() { return soundControl.turnOnVoice(2); };
    actionsMatrix[1][3] = [this]() { return soundControl.turnOffVoice(); };
    actionsMatrix[1][4] = [this]() { return heatControl.turnOnHeat(); };
    actionsMatrix[1][5] = [this]() { return heatControl.turnOffHeat(); };
}


// Function to execute an action
bool ActionHandler::executeAction(int menuType, int actionIndex) {
    // Check if the indices are within valid ranges
    if (menuType < 0 || menuType >= 2 || actionIndex < 0 || actionIndex >= 6  || (menuType == 0 && actionIndex == 5)) {
        Serial.println("Invalid menu type or action index.");
        return false;
    }

    // Check if the function exists
    if (actionsMatrix[menuType][actionIndex]) {
        Serial.println("Executing action."+String(menuType)+String(actionIndex));

        return actionsMatrix[menuType][actionIndex](); // Call the function and return the value
    }
    else
    {
        Serial.println("Invalid action.");
        return false; // Return failure
    }
}

bool ActionHandler::executeTestScenario() {
    if (lightControl.turnOnLED() && soundControl.turnOnVoice(1) && heatControl.turnOnHeat()) {
        if (xTaskCreate(
                testScenarioTask,     // Task function
                "TestScenarioTask",   // Task name
                2048,                 // Stack size
                this,                  // Parameter to pass to the task
                1,                    // Task priority
                nullptr) == pdPASS) {
            return true;
        } else {
            lightControl.turnOffLED();
            soundControl.turnOffVoice();
            heatControl.turnOffHeat();
            return false;
        }
    }
    return false;
}

// Function to read and calculate battery percentage
bool ActionHandler::readBatteryPercentage() {
    float sum = 0;
    for (int i = 0; i < MAX_SAMPLES; i++) {
      sum += samples[i];
    }
    filtered_battery_voltage = sum / MAX_SAMPLES;

    // Calculate the battery percentage based on the filtered voltage
    filtered_battery_percent = (filtered_battery_voltage - 11.0) / (12.6 - 11.0) * 100.0;
    filtered_battery_percent = constrain(filtered_battery_percent, 0, 100);
    return true;
}


// Function to turn off the system
bool ActionHandler::turnOffSystem() {
    // Implement the logic to turn off the system
    if( lightControl.turnOffLED() && soundControl.turnOffVoice() && heatControl.turnOffHeat() )
            return true;

    return false;
}

// Scenario 1 represents a wartime situation designed to draw attention and create the illusion of heightened activity and presence.
bool ActionHandler::executeScenario1() {
    // Implement the logic for scenario 1
    // i want to play with the components so i will turn them on and off but not in a sequence like test scenario 
    
    Serial.println("Executing Scenario 1");
    // Add your logic here
    return true;
}

// Scenario 2 represents a peaceful, everyday routine, simulating a calm and relaxed atmosphere
bool ActionHandler::executeScenario2() {
    // Implement the logic for scenario 2
    Serial.println("Executing Scenario 2");
    // Add your logic here
    return true;
}


// Task function to handle the delay and turn off the components
void ActionHandler::testScenarioTask(void* param) {
    ActionHandler* self = static_cast<ActionHandler*>(param);  // Cast the parameter to the class type

    unsigned long startTime = millis();

    while (millis() - startTime < 10000) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    // try to turn off the components 
    if (self->lightControl.turnOffLED()  && self->heatControl.turnOffHeat() && self->soundControl.turnOffVoice()) {
        Serial.println("Components turned off successfully.");
    }

    vTaskDelete(nullptr);
}



void ActionHandler::readBatteryVoltage() {
    int raw = analogRead(BATTERY_ADC_PIN);
    float adc_voltage = (raw / 4095.0) * 3.3;
    float battery_voltage = adc_voltage * (131.22 / 31.22);  // יחס מחלק המתח
    sample_index = (sample_index + 1) % MAX_SAMPLES; // Ensure sample_index wraps around
    if(sample_index == 0) {
        buffer_filled = true; // Buffer is filled after the first full cycle
    }   
    samples[sample_index] = battery_voltage;
}
