#ifndef HEATCONTROL_H
#define HEATCONTROL_H

#include <Adafruit_Sensor.h>  // Include the common sensor library from Adafruit
#include <DHT.h>              // Include the DHT sensor library
#include <freertos/task.h>

extern bool heatingActive;  // Declaration only, not definition of the global variable

// HeatControl class
class HeatControl {
public:
    // Constructor to initialize the MOSFET pin and sensor pin
    HeatControl(int heatPin, int sensorPin, int targetTemperature = 37);

    bool turnOnHeat();     // Function to turn on the heating with temperature monitoring
    bool turnOffHeat();    // Function to turn off the heating

private:
    int heatPin;          // Pin connected to the MOSFET
    int sensorPin;        // Pin connected to the temperature sensor
    int targetTemperature; // Target temperature
    DHT dht;               // DHT sensor object
    TaskHandle_t heatingTaskHandle; // Task handle for the heating control loop
    void heatControlLoop(); // Heating control loop
};

#endif