#include "Barrier.h"

void Barrier::setup() {
    _servo.attach(PIN_BARRIER_SERVO);
    close(); // Start with the barrier closed
    // Serial.println("Barrier setup complete.");
}

void Barrier::open() {
    if (!_isOpen) {
        _servo.write(BARRIER_OPEN_ANGLE);
        delay(BARRIER_DELAY_MS); // Wait for servo to reach position
        _isOpen = true;
        // Serial.println("Barrier opened.");
    }
}

void Barrier::close() {
    if (_isOpen) {
        _servo.write(BARRIER_CLOSED_ANGLE);
        delay(BARRIER_DELAY_MS); // Wait for servo to reach position
        _isOpen = false;
        // Serial.println("Barrier closed.");
    }
}
