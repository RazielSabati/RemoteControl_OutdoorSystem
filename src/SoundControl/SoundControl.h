#ifndef SOUNDCONTROL_H
#define SOUNDCONTROL_H

#include <Arduino.h>
#include <HardwareSerial.h>

class SoundControl {
public:
    SoundControl(int pin, HardwareSerial& mp3Serial);  // Constructor with pin number and UART channel
    bool turnOnVoice();    // Turn on the voice
    bool turnOffVoice();   // Turn off the voice

private:
    int soundPin;          // Pin number for sound control
    HardwareSerial& MP3;   // Reference to the HardwareSerial object for MP3 communication
    bool playTrack(uint16_t trackNumber); // Play a track by its number
    bool setVolume(uint8_t volume);       // Set the volume level
    bool sendCommand(uint8_t *command, size_t len);  // Send a command to the MP3 module

    // Predefined commands for the MP3 module:
    const uint8_t CMD_PLAY_TRACK[8] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0xEF}; // Command to play a track
    const uint8_t CMD_SET_VOLUME[8] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x00, 0xEF}; // Command to set volume
    const uint8_t CMD_STOP[8] = {0x7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xEF};       // Command to stop playback
};

#endif