#include "ActionHandler.h"
#include <Arduino.h>

ActionHandler::ActionHandler() 
    : lightControl(), heatControl(15), soundControl(16) {  // יצירת מופעים עם ערכי ברירת מחדל
    

    // הגדר את המטריצה
    actionsMatrix[0][0] = []() { Serial.println("Executing Test scenario"); };
    actionsMatrix[0][1] = []() { Serial.println("Reading battery percentage"); };
    actionsMatrix[0][2] = []() { Serial.println("System turning off"); };
    actionsMatrix[0][3] = []() { Serial.println("Executing Scenario 1"); };
    actionsMatrix[0][4] = []() { Serial.println("Executing Scenario 2"); };
    actionsMatrix[0][5] = nullptr;

    actionsMatrix[1][0] = [this]() { lightControl.turnOnLED(); };
    actionsMatrix[1][1] = [this]() { lightControl.turnOffLED(); };
    actionsMatrix[1][2] = [this]() { soundControl.turnOnVoice(); };
    actionsMatrix[1][3] = [this]() { soundControl.turnOffVoice(); };
    actionsMatrix[1][4] = [this]() { heatControl.turnOnHeat(); };
    actionsMatrix[1][5] = [this]() { heatControl.turnOffHeat(); };
}

void ActionHandler::executeAction(int menuType, int actionIndex) {
    if (actionsMatrix[menuType][actionIndex]) {
        actionsMatrix[menuType][actionIndex]();
    } else {
        Serial.println("Invalid action.");
    }
}


