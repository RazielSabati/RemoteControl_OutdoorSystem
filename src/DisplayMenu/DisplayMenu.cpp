#include "DisplayMenu.h"

DisplayMenu::DisplayMenu() {}

TaskHandle_t DisplayMenu::clearMessageTaskHandle = NULL;

void DisplayMenu::setupScreen() {
    Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C connection
    this->display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Initialize display with initialization list

    // Initialize the display and check for success
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true); // Stop the program if initialization fails
    }
    display.clearDisplay();
    displayReceivedMessage("System ready");
}

// Task function that will run on the other core
void DisplayMenu::clearMessageTask(void* parameter) {
    DisplayMenu* menu = (DisplayMenu*)parameter;
    
    // Delay for 4 seconds
    vTaskDelay(pdMS_TO_TICKS(4000));
    
    // Clear the area
    menu->display.fillRect(0, 0, SCREEN_WIDTH, 16, BLACK);
    menu->display.display();
    
    // Delete the task
    clearMessageTaskHandle = NULL;
    vTaskDelete(NULL);
}

void DisplayMenu::displayReceivedMessage(String confirmationMessage) {
    // Clear entire display for full-screen message
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    int lineHeight = 8;  // Height of each line in pixels
    int currentLine = 0; // Track the current line

    for (int i = 0; i < confirmationMessage.length(); i++) {
        char c = confirmationMessage.charAt(i);
        
        display.print(c);

        // Check if current position is past screen width
        if (display.getCursorX() >= SCREEN_WIDTH) {
            // Move to next line if we hit the edge of the screen
            currentLine++;
            display.setCursor(0, currentLine * lineHeight);

            // Stop displaying if we reach the bottom of the screen
            if ((currentLine + 1) * lineHeight >= SCREEN_HEIGHT) {
                break;
            }
        }
    }

    display.display();
}


