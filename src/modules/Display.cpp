#include "Display.h"

// Constructor for LiquidCrystal_I2C
Display::Display() : _lcd(LCD_ADDR, LCD_COLS, LCD_ROWS) {}

void Display::setup() {
    // Initialize LCD
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
    _lcd.print("System Init...");

    // Initialize LED pins
    for (uint8_t i = 0; i < NUM_SLOTS; ++i) {
        pinMode(PINS_LED_GREEN[i], OUTPUT);
        pinMode(PINS_LED_RED[i], OUTPUT);
        setSlotLED(i, GREEN); // Default to GREEN (available)
    }
    // Serial.println("Display setup complete.");
}

void Display::clearLine(uint8_t line) {
    _lcd.setCursor(0, line);
    for (uint8_t i = 0; i < LCD_COLS; ++i) {
        _lcd.print(" ");
    }
}

void Display::print(const char* message, uint8_t line, bool clearLine) {
    if (line >= LCD_ROWS) return; // Basic bounds check

    if (clearLine) {
        clearLine(line);
    }
    _lcd.setCursor(0, line);
    _lcd.print(message);
}

// Overload for String objects
void Display::print(String message, uint8_t line, bool clearLine) {
    print(message.c_str(), line, clearLine);
}

void Display::clear() {
    _lcd.clear();
}

void Display::setSlotLED(uint8_t slot, LedState state) {
    if (slot >= NUM_SLOTS) return; // Basic bounds check

    // Turn off both LEDs first
    digitalWrite(PINS_LED_GREEN[slot], LOW);
    digitalWrite(PINS_LED_RED[slot], LOW);

    // Turn on the selected LED
    switch (state) {
        case GREEN:
            digitalWrite(PINS_LED_GREEN[slot], HIGH);
            break;
        case RED:
            digitalWrite(PINS_LED_RED[slot], HIGH);
            break;
        case OFF:
            // Already off
            break;
        case FLASHING_GREEN: // Basic implementation (just turns on green)
             // TODO: Implement flashing logic (requires periodic calls)
            digitalWrite(PINS_LED_GREEN[slot], HIGH);
            break;
        case FLASHING_RED: // Basic implementation (just turns on red)
            // TODO: Implement flashing logic (requires periodic calls)
             digitalWrite(PINS_LED_RED[slot], HIGH);
            break;
    }
    // Optional: Store state if implementing flashing
    // _currentLedState[slot] = state;
}

/* Optional: Update method for non-blocking flashing
void Display::update() {
    unsigned long currentMillis = millis();
    for (uint8_t i = 0; i < NUM_SLOTS; ++i) {
        if (_currentLedState[i] == FLASHING_GREEN || _currentLedState[i] == FLASHING_RED) {
            if (currentMillis - _lastFlashTime[i] >= 500) { // Flash interval (e.g., 500ms)
                _lastFlashTime[i] = currentMillis;
                _ledFlashState[i] = !_ledFlashState[i]; // Toggle state

                if (_currentLedState[i] == FLASHING_GREEN) {
                    digitalWrite(PINS_LED_GREEN[i], _ledFlashState[i] ? HIGH : LOW);
                    digitalWrite(PINS_LED_RED[i], LOW);
                } else { // FLASHING_RED
                    digitalWrite(PINS_LED_RED[i], _ledFlashState[i] ? HIGH : LOW);
                    digitalWrite(PINS_LED_GREEN[i], LOW);
                }
            }
        }
    }
}
*/
