#ifndef PLATFORM_H
#define PLATFORM_H

#include "../config.h"
#include <Arduino.h>
#include <Servo.h> // Include the Servo library

class Platform {
public:
    void setup();
    // Rotates the platform servo to the angle defined for the target slot
    // Returns true if rotation command was sent, false if slot index is invalid.
    bool rotateToSlot(uint8_t slot);
    // Optional: Rotate to a specific angle (degrees)
    // void rotateToAngle(int angle);

    // Optional: Check if rotation is complete (useful for non-blocking)
    // bool isRotationComplete(); 

private:
    Servo _servo;      // Servo object for platform rotation
    int _currentAngle = -1; // Keep track of the current angle (optional)
    int _targetAngle = -1; // Target angle for rotation (optional)
};

#endif // PLATFORM_H
