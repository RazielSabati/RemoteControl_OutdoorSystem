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
    std::function<void()> actionsMatrix[2][6];

public:
    ActionHandler();
    void executeAction(int menuType, int actionIndex);
};

#endif
