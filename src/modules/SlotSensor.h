#ifndef SLOT_SENSOR_H
#define SLOT_SENSOR_H

#include "../config.h"
#include <Arduino.h>

class SlotSensor {
public:
    void setup();
    bool isSlotFree(uint8_t slot); // Returns true if distance > threshold
    float getSlotDistanceCm(uint8_t slot); // Returns distance in CM
    int findFirstFreeSlot(); // Returns index of first free slot, or -1 if none

private:
    // Function to read distance from a single sensor
    float readDistanceCm(uint8_t trigPin, uint8_t echoPin);
    
    // Optional: Array for median filter
    // static const int MEDIAN_FILTER_SIZE = 5;
    // float _distanceReadings[MEDIAN_FILTER_SIZE];
    // float getMedianDistanceCm(uint8_t slot);
};

#endif // SLOT_SENSOR_H
