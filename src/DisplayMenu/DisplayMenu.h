#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // We will use the OLED without a RESET pin
#define MAX_MESSAGES 4 // Maximum number of messages we want to display
// Instead of the current code using pins 21, 22
#define SDA_PIN 25  // For example
#define SCL_PIN 33  // For example

class DisplayMenu {
  private:
    // Matrix of menu items
    const char* namesMatrix[2][6] = {
      { "test scenario", "read battery %", "turn off system", "scenario 1", "scenario 2", "manual control" },
      { "turn on led", "turn off led", "turn on voice", "turn off voice", "turn on heat", "turn off heat" }
    };

    Adafruit_SSD1306 display;

    static TaskHandle_t clearMessageTaskHandle;

  public:
    DisplayMenu(); // Constructor
    void setupScreen(); // Function to set up the screen
    void displayTopScreen(String Message); // Function to display a message at the top of the screen
    void updateBottomScreen(String Message); // Function to display a message at the bottom of the screen
    void updateMiddleScreen(String Message); // Function to display a message in the middle of the screen
    const char* getData(int externalIndex, int internalIndex); // Function to get data from the menu matrix
};

#endif // DISPLAYMENU_H