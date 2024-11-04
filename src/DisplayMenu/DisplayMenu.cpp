#include "DisplayMenu.h"

// Constructor of the DisplayMenu class
DisplayMenu::DisplayMenu() {}

// Function to set up the screen
void DisplayMenu::setupScreen() {
    Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C connection
    this->display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Initialize display with initialization list

    // Initialize the display and check for success
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true); // Stop the program if initialization fails
    }
    display.clearDisplay();
    displayTopScreen("System ready");
}

// Function to get data from the menu matrix
const char* DisplayMenu::getData(int externalIndex, int internalIndex) {
    return namesMatrix[externalIndex][internalIndex];
}

// Function to display a message at the top of the screen
void DisplayMenu::displayTopScreen(String confirmationMessage) {
    display.fillRect(0, 0, SCREEN_WIDTH, 16, BLACK);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print(confirmationMessage);
    display.display();
}

// Function to display a message in the middle of the screen
void DisplayMenu::updateMiddleScreen(String confirmationMessage) {
    display.fillRect(0, 20, SCREEN_WIDTH, SCREEN_HEIGHT - 20, BLACK); // Clear the middle part of the screen
    display.setCursor(0, 20); // Set the cursor position to start printing in the middle
    display.setTextSize(1); // Set text size
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

// Function to display a message at the bottom of the screen
void DisplayMenu::updateBottomScreen(String confirmationMessage) {
    display.fillRect(0, 48, SCREEN_WIDTH, SCREEN_HEIGHT - 48, BLACK); // Clear the bottom part of the screen
    display.setCursor(0, 48); // Set the cursor position to start printing at the bottom
    display.setTextSize(1); // Set text size
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