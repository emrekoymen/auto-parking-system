#ifndef TIMER_H
#define TIMER_H

#include "../config.h"
#include <Arduino.h>

// Forward declaration if Display class is used for logging inside Timer
// class Display; 

struct ParkingTimer {
    unsigned long startTs[NUM_SLOTS] = {0};
    unsigned long durationMs[NUM_SLOTS] = {0};
    bool isRunning[NUM_SLOTS] = {false};

    // void setup(Display& display); // Optional: Link to display for logging
    void start(uint8_t slot);
    void stop(uint8_t slot);
    unsigned long getDurationSeconds(uint8_t slot);
    void reset(uint8_t slot);

private:
    // Display* _display = nullptr; // Optional: Pointer to display instance
    // void logToLCD(uint8_t slot, unsigned long durationMs); // Make private if used
};

#endif // TIMER_H
