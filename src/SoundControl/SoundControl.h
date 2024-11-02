#ifndef SOUNDCONTROL_H
#define SOUNDCONTROL_H
#include <Arduino.h>

class SoundControl {
public:
    SoundControl(int pin);  // אתחול עם מספר הפין
    void turnOnVoice();
    void turnOffVoice();
    // פונקציות נוספות כמו playSound אפשר להוסיף פה
private:
    int soundPin;
};

#endif
