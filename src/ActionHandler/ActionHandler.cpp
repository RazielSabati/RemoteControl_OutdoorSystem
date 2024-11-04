#include "ActionHandler.h"
#include <Arduino.h>

ActionHandler::ActionHandler() 
    : lightControl(), heatControl(15), soundControl(16, Serial1) {  // יצירת מופעים עם ערכי ברירת מחדל
    
    // הגדרת מטריצת הפעולות עם פונקציות שמחזירות bool
    actionsMatrix[0][0] = []() { Serial.println("Executing Test scenario"); return true; };
    actionsMatrix[0][1] = []() { Serial.println("Reading battery percentage"); return true; };
    actionsMatrix[0][2] = []() { Serial.println("System turning off"); return true; };
    actionsMatrix[0][3] = []() { Serial.println("Executing Scenario 1"); return true; };
    actionsMatrix[0][4] = []() { Serial.println("Executing Scenario 2"); return true; };
    actionsMatrix[0][5] = nullptr; // לא פעילה

    actionsMatrix[1][0] = [this]() { return lightControl.turnOnLED(); };
    actionsMatrix[1][1] = [this]() { return lightControl.turnOffLED(); };
    actionsMatrix[1][2] = [this]() { return soundControl.turnOnVoice(); };
    actionsMatrix[1][3] = [this]() { return soundControl.turnOffVoice(); };
    // actionsMatrix[1][4] = [this]() { return heatControl.turnOnHeat(); };
    // actionsMatrix[1][5] = [this]() { return heatControl.turnOffHeat(); };
}

// פונקציה לביצוע פעולה
bool ActionHandler::executeAction(int menuType, int actionIndex) {
    // בדוק אם יש טווחים נכונים
    if (menuType < 0 || menuType >= 2 || actionIndex < 0 || actionIndex >= 6  || (menuType == 0 && actionIndex == 5)) {
        Serial.println("Invalid menu type or action index.");
        return false;
    }

    // בדוק אם הפונקציה קיימת
    if (actionsMatrix[menuType][actionIndex]) {
        return actionsMatrix[menuType][actionIndex](); // קריאה לפונקציה והחזרת הערך
    } else {
        Serial.println("Invalid action.");
        return false; // החזר כישלון
    }
}
