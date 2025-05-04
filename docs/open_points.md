# Open Points & Design Decisions

This document lists areas in the Auto-Parking System that require clarification, configuration decisions, or further implementation details.

## 1. Undefined Logic / Behavior

*   **`WEIGHT_CHECK` State Interpretation:**
    *   The state machine diagram mentions `checkVehicleWeight()`, but the hardware list has no weight sensor.
    *   The current `ParkingSystem.ino` immediately checks `slotSensor.findFirstFreeSlot()` upon entering this state.
    *   **Decision:** Confirm that this state is purely about checking *slot availability* using ultrasonic sensors after the entry IR is triggered, rather than checking vehicle presence/weight via other means. The current implementation assumes this.

*   **Error Handling:**
    *   What should the system do if a sensor fails (e.g., `SlotSensor` returns invalid distance, IR sensor stuck)?
    *   How should servo failures be handled (e.g., if `Barrier` or `Platform` doesn't reach the target)?
    *   How to handle communication issues (Bluetooth commands failing, I2C errors in two-board setup)?
    *   **Decision:** Define specific error states or recovery mechanisms (e.g., LCD message, specific LED pattern, attempt retry, enter safe mode).

*   **`GUIDE` State Details:**
    *   The state machine calls for buzzer chirps and flashing LEDs.
    *   `ParkingSystem.ino` currently just sets the LED green (`display.setSlotLED(targetSlot, GREEN);`) and does a single short `beep(50);` upon entering the state.
    *   **Decision:** Define the exact pattern for flashing (e.g., 500ms on/off) and beeping (e.g., short beeps every second) while in this state. This requires non-blocking timing logic (using `millis()`) instead of `delay()` within the `GUIDE` case in `loop()`.

*   **Barrier Safety Interlock:**
    *   `README.md` mentions a safety interlock.
    *   **Decision:** Define the exact conditions. Should the barrier refuse to close if the *entry* IR (`PIN_IR_ENTRY`) is blocked after the `BARRIER_OPEN` state? Should it stop/re-open if the *exit* IR (`PIN_IR_EXIT`) is blocked while closing in the `EXIT` state?

*   **`FULL` State Timeout:**
    *   The state machine says exit on timeout or slot free. `ParkingSystem.ino` implements a simple 5-second timeout.
    *   **Decision:** Confirm if 5 seconds is appropriate, or if this should be configurable in `config.h`.

*   **Manual Bluetooth Rotation (`handleRotateCommand`):**
    *   Currently, this command directly writes to the platform servo, bypassing the state machine.
    *   **Decision:** Should this command be allowed only in certain states (e.g., `IDLE`) or disabled entirely? Should it update the system's understanding of the platform's position?

## 2. Potential Wiring & Calibration Issues

*   **Pin Mapping:** Double-check all physical connections against `src/config.h`.
*   **I2C (Two-Board Setup):** Requires careful wiring (SDA, SCL, GND), correct slave address, and potentially pull-up resistors. Needs corresponding slave code.
*   **Sensor Calibration (`SlotSensor`):** The `SLOT_OCCUPIED_THRESHOLD_CM` (currently 20.0 cm) in `config.h` likely needs tuning based on the physical setup.
*   **IR Sensor Behavior:** Confirm if sensors output LOW or HIGH when the beam is broken and ensure `pinMode` (INPUT_PULLUP) or external resistors match.
*   **Servo Calibration (`Barrier`, `Platform`):** The `BARRIER_OPEN_ANGLE`, `BARRIER_CLOSED_ANGLE`, and `PLATFORM_SLOT_ANGLES` in `config.h` must be calibrated for the physical range of motion.
*   **Power Supply:** Ensure the PSU can handle the combined current draw, especially peak servo/motor current.
*   **Bluetooth Logic Level:** Verify if HC-05 needs 3.3V logic level shifting when connected to the 5V Arduino UNO TX pin. 