#include "ActionHandler.h"
#include <Arduino.h>

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
    actionsMatrix[1][2] = [this]() { return soundControl.turnOnVoice(); };
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
    } else {
        Serial.println("Invalid action.");
        return false; // Return failure
    }
}



// Function to execute a test scenario
bool ActionHandler::executeTestScenario() {
    // Implement the logic for the test scenario
    Serial.println("Executing Test scenario");
    // Add your logic here
    return true;
}

// Function to read battery percentage
bool ActionHandler::readBatteryPercentage() {
    // Implement the logic to read battery percentage
    Serial.println("Reading battery percentage");
    // Add your logic here
    return true;
}

// Function to turn off the system
bool ActionHandler::turnOffSystem() {
    // Implement the logic to turn off the system
    Serial.println("System turning off");
    // Add your logic here
    return true;
}

// Function to execute scenario 1
bool ActionHandler::executeScenario1() {
    // Implement the logic for scenario 1
    Serial.println("Executing Scenario 1");
    // Add your logic here
    return true;
}

// Function to execute scenario 2
bool ActionHandler::executeScenario2() {
    // Implement the logic for scenario 2
    Serial.println("Executing Scenario 2");
    // Add your logic here
    return true;
}

