#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- Pin Definitions ---

// Barrier Servo
const uint8_t PIN_BARRIER_SERVO = 9;

// Platform Servo/Motor
const uint8_t PIN_PLATFORM_SERVO = 10; // Or motor driver pins if using DC motor

// Ultrasonic Sensors (HC-SR04) - Assuming 3 slots
const uint8_t NUM_SLOTS = 3;
const uint8_t PINS_TRIG[NUM_SLOTS] = {2, 4, 7};
const uint8_t PINS_ECHO[NUM_SLOTS] = {3, 5, 8};

// IR Break-beam Sensors
const uint8_t PIN_IR_ENTRY = A0;
const uint8_t PIN_IR_EXIT = A1;

// Status LEDs (Green/Red per slot)
const uint8_t PINS_LED_GREEN[NUM_SLOTS] = {11, 12, 13}; // Example pins
const uint8_t PINS_LED_RED[NUM_SLOTS] = {A2, A3, A4};   // Example pins

// Buzzer
const uint8_t PIN_BUZZER = 6;

// LCD (I2C) - Address might vary
const uint8_t LCD_ADDR = 0x27; // Common address, check yours
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;

// Bluetooth (HC-05) - Using SoftwareSerial or Hardware Serial
// If using pins 0, 1 (Hardware Serial), connect TX->RX, RX->TX
// const uint8_t PIN_BT_RX = 0; // Or specific pins for SoftwareSerial
// const uint8_t PIN_BT_TX = 1; // Or specific pins for SoftwareSerial


// --- Thresholds & Settings ---

// Slot Sensor distance threshold (cm) for occupied/free
const float SLOT_OCCUPIED_THRESHOLD_CM = 20.0;

// Barrier Servo angles
const int BARRIER_OPEN_ANGLE = 90;
const int BARRIER_CLOSED_ANGLE = 0;
const int BARRIER_DELAY_MS = 500; // Delay after moving barrier

// Platform Rotation (Example: Target angles for slots)
const int PLATFORM_SLOT_ANGLES[NUM_SLOTS] = {30, 90, 150};
const int PLATFORM_ROTATION_SPEED = 5; // Example speed factor

// Debounce delay for IR sensors (ms)
const unsigned long IR_DEBOUNCE_DELAY_MS = 50;

// Serial Monitor Baud Rate
const unsigned long SERIAL_BAUD_RATE = 9600;

#endif // CONFIG_H
