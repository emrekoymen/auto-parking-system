#include "Platform.h"

void Platform::setup() {
    _servo.attach(PIN_PLATFORM_SERVO);
    // Optionally, move to a default starting position, e.g., slot 0 or a neutral angle
    // rotateToSlot(0); 
    // Or set a specific angle, e.g., 90 degrees
    int initialAngle = PLATFORM_SLOT_ANGLES[1]; // Start facing middle slot
    _servo.write(initialAngle);
    _currentAngle = initialAngle;
    _targetAngle = initialAngle;
    delay(500); // Allow servo to reach initial position
    // Serial.println("Platform setup complete.");
}

bool Platform::rotateToSlot(uint8_t slot) {
    if (slot >= NUM_SLOTS) {
        // Serial.print("Invalid slot index for platform rotation: "); Serial.println(slot);
        return false; // Invalid slot index
    }

    _targetAngle = PLATFORM_SLOT_ANGLES[slot];
    // Serial.print("Rotating platform to slot "); Serial.print(slot);
    // Serial.print(" (Angle: "); Serial.print(_targetAngle); Serial.println(")");

    // Simple blocking rotation using write() and delay
    // For smoother or non-blocking rotation, you'd need to increment angle in loop
    _servo.write(_targetAngle);
    _currentAngle = _targetAngle; // Assume it reaches instantly for blocking

    // Add a delay to allow the servo to physically move
    // This delay might need tuning based on servo speed and load
    // A more sophisticated approach would check servo.read() until target is met
    delay(1000); // Adjust delay as needed (e.g., 1 second)

    return true;
}

/*
// Optional: Rotate to a specific angle
void Platform::rotateToAngle(int angle) {
    // Add constraints if necessary (e.g., 0-180 degrees)
    angle = constrain(angle, 0, 180);
    _targetAngle = angle;
    _servo.write(_targetAngle);
    _currentAngle = _targetAngle; // Assume instant move
    delay(1000); // Adjust delay
    // Serial.print("Rotating platform to angle: "); Serial.println(angle);
}
*/

/*
// Optional: Non-blocking rotation check
// Requires updating _currentAngle gradually in main loop or Platform::update()
// And not using delay() in rotateToSlot/rotateToAngle
bool Platform::isRotationComplete() {
    // Read the current servo position (may not be perfectly accurate)
    // _currentAngle = _servo.read(); 
    return abs(_currentAngle - _targetAngle) < 2; // Allow small tolerance
}
*/
