#ifndef DISPLAY_H
#define DISPLAY_H

#include "../config.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // Library for I2C LCD
#include <Wire.h>             // Required for I2C

// Enum for LED colors/states
enum LedState {
    OFF,
    GREEN,
    RED,
    FLASHING_GREEN, // Add more states if needed (e.g., flashing)
    FLASHING_RED
};

class Display {
public:
    void setup();
    void print(const char* message, uint8_t line = 0, bool clearLine = true);
    void print(String message, uint8_t line = 0, bool clearLine = true);
    void setSlotLED(uint8_t slot, LedState state);
    void clear();
    // void update(); // Optional: for handling flashing LEDs without blocking

private:
    LiquidCrystal_I2C _lcd;
    // Optional: Add state for flashing LEDs
    // unsigned long _lastFlashTime[NUM_SLOTS] = {0};
    // bool _ledFlashState[NUM_SLOTS] = {false};
    // LedState _currentLedState[NUM_SLOTS] = {OFF};

    void clearLine(uint8_t line);
};

#endif // DISPLAY_H
