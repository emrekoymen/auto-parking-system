#include "SlotSensor.h"

void SlotSensor::setup() {
    for (uint8_t i = 0; i < NUM_SLOTS; ++i) {
        pinMode(PINS_TRIG[i], OUTPUT);
        pinMode(PINS_ECHO[i], INPUT);
    }
    // Serial.println("SlotSensor setup complete.");
}

// Reads distance from a single HC-SR04 sensor
float SlotSensor::readDistanceCm(uint8_t trigPin, uint8_t echoPin) {
    // Clear the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Send a 10us pulse to trigger
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echoPin, returns the sound wave travel time in microseconds
    // Timeout added for robustness (e.g., 30000 us corresponds to ~5 meters)
    long duration = pulseIn(echoPin, HIGH, 30000); 

    // Calculate the distance
    // Speed of sound = 343 m/s = 0.0343 cm/us
    // Distance = (Travel Time / 2) * Speed of Sound
    float distance = (duration * 0.0343) / 2.0;

    // Handle timeout or out-of-range readings
    if (duration == 0 || distance > 400) { // Max range of HC-SR04 is ~400cm
        return -1.0; // Indicate error or out of range
    }

    return distance;
}

// Returns the raw distance reading for a specific slot
float SlotSensor::getSlotDistanceCm(uint8_t slot) {
    if (slot >= NUM_SLOTS) return -1.0; // Invalid slot

    // --- Basic Reading (No Filter) ---
    return readDistanceCm(PINS_TRIG[slot], PINS_ECHO[slot]);

    // --- Placeholder for Median Filter ---
    // return getMedianDistanceCm(slot);
}

// Checks if a specific slot is free based on the threshold
bool SlotSensor::isSlotFree(uint8_t slot) {
    float distance = getSlotDistanceCm(slot);
    // Consider slot free if reading is valid (>0) and above the threshold
    return (distance > 0 && distance > SLOT_OCCUPIED_THRESHOLD_CM);
}

// Finds the first available slot (index 0 to NUM_SLOTS-1)
int SlotSensor::findFirstFreeSlot() {
    for (uint8_t i = 0; i < NUM_SLOTS; ++i) {
        if (isSlotFree(i)) {
            return i; // Return the index of the first free slot
        }
    }
    return -1; // Return -1 if no slots are free
}

/*
// --- Optional: Median Filter Implementation ---
// Helper function for sorting (used by median)
int compareFloats(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}

// Reads multiple times and returns the median distance
float SlotSensor::getMedianDistanceCm(uint8_t slot) {
    if (slot >= NUM_SLOTS) return -1.0;

    uint8_t trigPin = PINS_TRIG[slot];
    uint8_t echoPin = PINS_ECHO[slot];
    int validReadings = 0;

    for (int i = 0; i < MEDIAN_FILTER_SIZE; ++i) {
        float dist = readDistanceCm(trigPin, echoPin);
        if (dist > 0) { // Only consider valid readings
            _distanceReadings[validReadings++] = dist;
        }
        delay(30); // Short delay between readings
    }

    if (validReadings == 0) {
        return -1.0; // No valid readings obtained
    }

    // Sort the valid readings
    qsort(_distanceReadings, validReadings, sizeof(float), compareFloats);

    // Return the median value
    return _distanceReadings[validReadings / 2];
}
*/
