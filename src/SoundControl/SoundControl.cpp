#include "SoundControl.h"

// Constructor of the class - initializes the MP3 module
SoundControl::SoundControl(int pin, HardwareSerial& mp3Serial) : soundPin(pin), MP3(mp3Serial) {
    pinMode(soundPin, OUTPUT);
    MP3.begin(9600, SERIAL_8N1, 16, 17); // Set up UART with 9600 baud rate, RX=16, TX=17
    
    delay(1000); // Wait for the module to initialize
    setVolume(20); // Set volume level to 20
    Serial.println("MP3 Module Initialized.");
}

// Function to turn on the voice
bool SoundControl::turnOnVoice() {
    digitalWrite(soundPin, HIGH);
    // Check if the sound was turned on successfully
    if (digitalRead(soundPin) == LOW) {   
        Serial.println("Failed to turn on sound.");
        return false;
    }
    if(playTrack(1)) {
        Serial.println("Sound turned on.");
        return true;
    }
    return false; // Play the first track
}

// Function to turn off the voice
bool SoundControl::turnOffVoice() {
    // Send stop command to the MP3 module
    if (!sendCommand((uint8_t*)CMD_STOP, sizeof(CMD_STOP))) {
        Serial.println("Failed to send stop command.");
        return false;
    }

    // Turn off the sound
    digitalWrite(soundPin, LOW);

    // Check if the sound was turned off successfully
    if (digitalRead(soundPin) == HIGH) {   
        Serial.println("Failed to turn off sound.");
        return false;
    }

    Serial.println("Sound turned off."); 
    return true; // Return true if the operation succeeded
}

// Play a track by its number
bool SoundControl::playTrack(uint16_t trackNumber) {
    uint8_t command[8];
    memcpy(command, CMD_PLAY_TRACK, 8);
    command[6] = trackNumber;
    if(sendCommand(command, sizeof(command)))
        {
            Serial.print("Playing track ");
            Serial.println(trackNumber);
            return true;
        }
    return false; ;
}

// Set the volume level
bool SoundControl::setVolume(uint8_t volume) {
    if (volume > 30) volume = 30; // Ensure the volume does not exceed 30
    uint8_t command[8]; // Create a buffer for the command
    memcpy(command, CMD_SET_VOLUME, 8); // Copy the predefined command to the buffer
    command[6] = volume; // Set the volume level in the command
    return sendCommand(command, sizeof(command)); // Send the command to the MP3 module
}

// Send a command to the MP3 module
bool SoundControl::sendCommand(uint8_t *command, size_t len) {
    size_t bytesWritten = MP3.write(command, len);
    if(bytesWritten != len) {
        Serial.println("Failed to send command.");
        return false; // Return false if not all bytes were written
    }

    return true; // Return true if all bytes were written
}