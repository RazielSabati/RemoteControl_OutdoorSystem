#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

#include <functional>
#include "../LightControl/LightControl.h"
#include "../HeatControl/HeatControl.h"
#include "../SoundControl/SoundControl.h"
#define BATTERY_ADC_PIN 35  // GPIO35
#define MAX_SAMPLES 50
#define MAX_DEVIATION 0.5
extern float samples[MAX_SAMPLES];


class ActionHandler {
private:
    LightControl lightControl;
    HeatControl heatControl;
    SoundControl soundControl;
    // Define a matrix of actions with functions that return bool
    std::function<bool()> actionsMatrix[2][6]; 

    int sample_index = 0;
    bool buffer_filled;
    
    float filtered_battery_voltage;
    float filtered_battery_percent;

public:
   

    ActionHandler();
    bool executeAction(int menuType, int actionIndex);
    bool executeTestScenario();
    bool readBatteryPercentage();
    bool turnOffSystem();
    bool executeScenario1();
    bool executeScenario2();
    static void testScenarioTask(void* param);
    uint8_t getBatteryPercentage(){
        return static_cast<uint8_t>(filtered_battery_percent);
    }


    void readBatteryVoltage();
    

};

#endif