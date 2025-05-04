#include "Timer.h"
// #include "Display.h" // Include if logging to LCD within Timer

/* Optional: Link Display instance for logging
void ParkingTimer::setup(Display& display) {
    _display = &display;
}
*/

void ParkingTimer::start(uint8_t slot) {
    if (slot < NUM_SLOTS && !isRunning[slot]) {
        startTs[slot] = millis();
        durationMs[slot] = 0; // Reset duration when starting
        isRunning[slot] = true;
        // Serial.print("Timer started for slot: "); Serial.println(slot);
    }
}

void ParkingTimer::stop(uint8_t slot) {
    if (slot < NUM_SLOTS && isRunning[slot]) {
        durationMs[slot] = millis() - startTs[slot];
        isRunning[slot] = false;
        // logToLCD(slot, durationMs[slot]); // Call internal log function if needed
        // Serial.print("Timer stopped for slot: "); Serial.print(slot);
        // Serial.print(", Duration: "); Serial.print(getDurationSeconds(slot)); Serial.println(" s");
    }
}

unsigned long ParkingTimer::getDurationSeconds(uint8_t slot) {
    if (slot < NUM_SLOTS) {
        if (isRunning[slot]) {
            // Return current elapsed time if still running
            return (millis() - startTs[slot]) / 1000;
        } else {
            // Return final duration if stopped
            return durationMs[slot] / 1000;
        }
    }
    return 0; // Invalid slot
}

void ParkingTimer::reset(uint8_t slot) {
    if (slot < NUM_SLOTS) {
        startTs[slot] = 0;
        durationMs[slot] = 0;
        isRunning[slot] = false;
    }
}

/* Optional: Implement LCD logging within Timer
void ParkingTimer::logToLCD(uint8_t slot, unsigned long stoppedDurationMs) {
    if (_display) { // Check if display is linked
        unsigned long totalSeconds = stoppedDurationMs / 1000;
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;

        char timeStr[9]; // HH:MM:SS\0
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);

        char line[17]; // 16 chars + null terminator
        snprintf(line, sizeof(line), "Slot %d Time:", slot + 1);
        _display->print(line, 0); // Print on line 0
        _display->print(timeStr, 1);    // Print on line 1
    }
}
*/
