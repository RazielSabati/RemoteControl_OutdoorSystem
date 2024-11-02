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
    // Display the message
    display.fillRect(0, 0, SCREEN_WIDTH, 16, BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print(confirmationMessage);
    display.display();
    
    // If there is a previous task still running, delete it
    if (clearMessageTaskHandle != NULL) {
        vTaskDelete(clearMessageTaskHandle);
    }
    
    // Create a new task on core 0
    xTaskCreatePinnedToCore(
        clearMessageTask,        // Task function
        "ClearMessage",          // Task name
        2048,                    // Stack size
        this,                    // Parameter to pass (this pointer)
        1,                       // Priority
        &clearMessageTaskHandle, // Task handle
        0                        // Core 0
    );
}
