#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

#include <functional>
#include "../LightControl/LightControl.h"
#include "../HeatControl/HeatControl.h"
#include "../SoundControl/SoundControl.h"

class ActionHandler {
private:
    LightControl lightControl;
    HeatControl heatControl;
    SoundControl soundControl;
    
    // Define a matrix of actions with functions that return bool
    std::function<bool()> actionsMatrix[2][6]; 

public:
    ActionHandler();
    bool executeAction(int menuType, int actionIndex);
    bool executeTestScenario();
    bool readBatteryPercentage();
    bool turnOffSystem();
    bool executeScenario1();
    bool executeScenario2();

};

#endif