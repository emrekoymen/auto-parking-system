#include "config.h"
#include "modules/Barrier.h"
#include "modules/SlotSensor.h"
#include "modules/Platform.h"
#include "modules/Display.h"
#include "modules/BluetoothCmd.h"
#include "modules/Timer.h"

// --- Module Objects ---
Barrier barrier;
SlotSensor slotSensor;
Platform platform;
Display display;
BluetoothCmd bluetoothCmd;
ParkingTimer parkingTimer;

// --- State Machine Definition ---
enum SystemState {
    IDLE,
    WEIGHT_CHECK, // Renamed from docs for clarity (IR check)
    BARRIER_OPEN,
    PLATFORM_ALIGN,
    GUIDE,
    PARKED,
    EXIT,
    FULL
};
SystemState currentState = IDLE;
int targetSlot = -1; // To store the assigned slot index

// --- IR Sensor Debouncing ---
// Entry Sensor
int irEntryState = HIGH;           // Current reading
int lastIrEntryState = HIGH;       // Previous reading
unsigned long lastEntryDebounceTime = 0; // Last time input pin was toggled
bool entryTriggered = false;       // Debounced state change flag (LOW -> HIGH)
bool vehicleEntered = false;     // Debounced state change flag (HIGH -> LOW)

// Exit Sensor
int irExitState = HIGH;
int lastIrExitState = HIGH;
unsigned long lastExitDebounceTime = 0;
bool exitTriggered = false;       // Debounced state change flag (LOW -> HIGH)
bool vehicleExited = false;      // Debounced state change flag (HIGH -> LOW)

// --- Forward Declarations for Bluetooth Callbacks ---
void handleRotateCommand(int angle);
void handleStatusCommand();

// --- Helper Functions ---
void changeState(SystemState newState); // Function to handle state transitions
void updateIrSensors();                // Function to read and debounce IR sensors
void beep(int durationMs);

// =================== SETUP ===================
void setup() {
    // Module Setups
    display.setup(); // Setup display first for messages
    barrier.setup();
    slotSensor.setup();
    platform.setup();
    // parkingTimer doesn't need a setup unless linked to display
    bluetoothCmd.setup(handleRotateCommand, handleStatusCommand); // Pass callbacks

    // Initialize IR Sensor Pins
    pinMode(PIN_IR_ENTRY, INPUT_PULLUP); // Use internal pull-up if needed
    pinMode(PIN_IR_EXIT, INPUT_PULLUP);

    // Initialize Buzzer Pin
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    // Initial State
    changeState(IDLE);

    // Short beep to indicate system ready
    beep(100);
}

// =================== LOOP ===================
void loop() {
    // 1. Check Inputs
    updateIrSensors();       // Read and debounce IR sensors
    bluetoothCmd.checkCommands(); // Check for Bluetooth commands

    // 2. Run State Machine Logic
    switch (currentState) {
        case IDLE:
            // Entry: Display "Waiting"
            // Exit Condition: Entry IR Triggered (LOW -> HIGH)
            if (entryTriggered) {
                changeState(WEIGHT_CHECK);
            }
            break;

        case WEIGHT_CHECK:
            // Entry: Check for free slot
            targetSlot = slotSensor.findFirstFreeSlot();
            if (targetSlot != -1) { // Free slot found
                 changeState(BARRIER_OPEN);
            } else { // No slots free
                changeState(FULL);
            }
            break;

        case BARRIER_OPEN:
            // Entry: Open barrier, Display "Welcome"
            // Exit Condition: Vehicle passes Entry IR (HIGH -> LOW)
            if (vehicleEntered) {
                barrier.close(); // Close barrier after vehicle passes
                 changeState(PLATFORM_ALIGN);
            }
            break;

        case PLATFORM_ALIGN:
            // Entry: Rotate platform to target slot
            // TODO: Handle rotation failure? (platform.rotateToSlot returns bool)
            platform.rotateToSlot(targetSlot);
            // Exit Condition: Rotation presumed complete (blocking call)
            changeState(GUIDE);
            break;

        case GUIDE:
            // Entry: Flash target slot GREEN LED, short beeps
            // (Flashing/beeps need non-blocking implementation or happen once)
            display.setSlotLED(targetSlot, FLASHING_GREEN); // Note: Currently just turns ON green
            beep(50);
            // Exit Condition: Slot sensor detects vehicle (distance < threshold)
            // Add a short delay to prevent immediate trigger if sensor reading fluctuates
            delay(100); 
            if (!slotSensor.isSlotFree(targetSlot)) {
                 changeState(PARKED);
            }
            break;

        case PARKED:
            // Entry: Set slot LED RED, start timer
            // Exit Condition: Exit IR Triggered (LOW -> HIGH)
             if (exitTriggered) {
                 changeState(EXIT);
             }
            break;

        case EXIT:
            // Entry: Open barrier, Stop timer, Show duration on LCD
            {
                unsigned long duration = parkingTimer.getDurationSeconds(targetSlot);
                int hours = duration / 3600;
                int mins = (duration % 3600) / 60;
                int secs = duration % 60;
                char timeStr[17];
                snprintf(timeStr, sizeof(timeStr), "Slot %d %02d:%02d:%02d", targetSlot + 1, hours, mins, secs);
                display.print("Park Duration:", 0);
                display.print(timeStr, 1);
            }
            // Exit Condition: Vehicle passes Exit IR (HIGH -> LOW)
            if (vehicleExited) {
                barrier.close();
                display.setSlotLED(targetSlot, GREEN); // Reset slot LED
                parkingTimer.reset(targetSlot); // Reset timer for the slot
                targetSlot = -1; // Clear target slot
                changeState(IDLE);
            }
            break;

        case FULL:
            // Entry: Display "No Space"
            // Exit Condition: Timeout (e.g., 5 seconds) OR a slot becomes free
            // Simple timeout implementation:
            {
                static unsigned long fullStartTime = 0;
                if (fullStartTime == 0) fullStartTime = millis();
                
                if (millis() - fullStartTime > 5000) { // 5 second timeout
                    fullStartTime = 0; // Reset for next time
                     changeState(IDLE);
                }
            }
            // Check if a slot becomes free
            if (slotSensor.findFirstFreeSlot() != -1) {
                 changeState(IDLE); // Go back to IDLE if space opens up
            }
            break;
    }

    // 3. Small delay to prevent high-speed looping (optional)
    delay(10); 
}

// =================== HELPER FUNCTIONS ===================

// --- State Transition Handler ---
void changeState(SystemState newState) {
    if (currentState == newState) return; // No change

    // Serial.print("State Change: ");
    // Serial.print(currentState); // Print previous state index
    // Serial.print(" -> ");
    // Serial.println(newState);   // Print new state index

    currentState = newState;
    // Reset IR trigger flags on state change
    entryTriggered = false;
    vehicleEntered = false;
    exitTriggered = false;
    vehicleExited = false;

    // Perform Entry Actions for the new state
    switch (currentState) {
        case IDLE:
            display.print("Auto Parking Sys", 0);
            display.print("Waiting...", 1);
            // Ensure all slot LEDs are in correct initial state (e.g., green if free)
            for(int i=0; i<NUM_SLOTS; ++i) {
                if(slotSensor.isSlotFree(i)) display.setSlotLED(i, GREEN);
                else display.setSlotLED(i, RED);
            }
            break;
        case WEIGHT_CHECK:
            display.print("Checking slots...", 0);
            display.print("", 1); 
            beep(50);
            break;
        case BARRIER_OPEN:
            display.print("Slot Found!", 0);
            display.print("Welcome!", 1);
            barrier.open();
            beep(100);
            break;
        case PLATFORM_ALIGN:
            display.print("Aligning Platform", 0);
            char msg[17];
            snprintf(msg, sizeof(msg), "To Slot %d...", targetSlot + 1);
            display.print(msg, 1);
            break;
        case GUIDE:
            display.print("Proceed to Slot", 0);
            snprintf(msg, sizeof(msg), "<<< Slot %d >>>", targetSlot + 1);
            display.print(msg, 1);
            // Flashing/beeping logic here if needed
            break;
        case PARKED:
            display.print("Vehicle Parked", 0);
            snprintf(msg, sizeof(msg), "In Slot %d", targetSlot + 1);
            display.print(msg, 1);
            display.setSlotLED(targetSlot, RED);
            parkingTimer.start(targetSlot);
            beep(200);
            break;
        case EXIT:
            display.print("Vehicle Leaving", 0);
            display.print("Opening barrier", 1);
            barrier.open();
            parkingTimer.stop(targetSlot);
            beep(100); delay(50); beep(100); // Double beep for exit
            break;
        case FULL:
            display.print("Sorry, Garage", 0);
            display.print("Is Full", 1);
            beep(500); // Longer beep for full
            break;
    }
}

// --- IR Sensor Update & Debounce ---
void updateIrSensors() {
    unsigned long currentMillis = millis();

    // --- Entry Sensor ---
    int readingEntry = digitalRead(PIN_IR_ENTRY);
    if (readingEntry != lastIrEntryState) {
        lastEntryDebounceTime = currentMillis; // Reset debounce timer
    }
    if ((currentMillis - lastEntryDebounceTime) > IR_DEBOUNCE_DELAY_MS) {
        // If the state has been stable for longer than the debounce delay
        if (readingEntry != irEntryState) {
            irEntryState = readingEntry;
            if (irEntryState == HIGH) {
                // LOW to HIGH transition (beam unblocked / vehicle arrived at sensor)
                entryTriggered = true; 
                // Serial.println("Entry IR Triggered (LOW->HIGH)");
            } else {
                // HIGH to LOW transition (beam blocked / vehicle passed sensor)
                 vehicleEntered = true;
                // Serial.println("Entry IR Passed (HIGH->LOW)");
            }
        }
    }
    lastIrEntryState = readingEntry;

    // --- Exit Sensor ---
     int readingExit = digitalRead(PIN_IR_EXIT);
     if (readingExit != lastIrExitState) {
         lastExitDebounceTime = currentMillis; // Reset debounce timer
     }
     if ((currentMillis - lastExitDebounceTime) > IR_DEBOUNCE_DELAY_MS) {
         if (readingExit != irExitState) {
             irExitState = readingExit;
             if (irExitState == HIGH) {
                 // LOW to HIGH transition (beam unblocked / vehicle arrived at sensor)
                 exitTriggered = true;
                // Serial.println("Exit IR Triggered (LOW->HIGH)");
             } else {
                 // HIGH to LOW transition (beam blocked / vehicle passed sensor)
                 vehicleExited = true;
                // Serial.println("Exit IR Passed (HIGH->LOW)");
             }
         }
     }
     lastIrExitState = readingExit;
}

// --- Buzzer Beep --- 
void beep(int durationMs) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(durationMs);
    digitalWrite(PIN_BUZZER, LOW);
}

// =================== BLUETOOTH CALLBACKS ===================

void handleRotateCommand(int angle) {
    // Example: Directly control platform servo via Bluetooth
    // Note: This bypasses the state machine logic for rotation.
    // Consider if this should only work in specific states or be disabled.
    // Serial.print("Bluetooth: Rotate command received: "); Serial.println(angle);
    
    // Constrain angle if needed
    angle = constrain(angle, 0, 180); 
    platform._servo.write(angle); // Accessing private member - better to add public method in Platform class
    display.print("Manual Rotate:", 0);
    char msg[17];
    snprintf(msg, sizeof(msg), "%d degrees", angle);
    display.print(msg, 1);
}

void handleStatusCommand() {
    // Example: Report current status via Bluetooth/Serial
    // Serial.println("--- System Status ---");
    // Serial.print("Current State: "); Serial.println(currentState); 
    // Serial.print("Target Slot: "); Serial.println(targetSlot);
    for (uint8_t i = 0; i < NUM_SLOTS; ++i) {
        // Serial.print("Slot "); Serial.print(i);
        // Serial.print(": ");
        float dist = slotSensor.getSlotDistanceCm(i);
        if (dist < 0) {
            // Serial.print("Sensor Error ");
        } else {
            // Serial.print(dist); Serial.print(" cm ");
        }
        if (slotSensor.isSlotFree(i)) {
            // Serial.print("(Free)");
        } else {
            // Serial.print("(Occupied)");
        }
        if (parkingTimer.isRunning[i]) {
           // Serial.print(" Timer: "); Serial.print(parkingTimer.getDurationSeconds(i)); Serial.print("s");
        } else if (parkingTimer.durationMs[i] > 0) {
             // Serial.print(" Last Time: "); Serial.print(parkingTimer.getDurationSeconds(i)); Serial.print("s");
        }
        // Serial.println();
    }
    // Serial.println("---------------------");

    // Send basic status to LCD as well
    display.print("Status Requested", 0);
    char statusLine[17];
    snprintf(statusLine, sizeof(statusLine), "State:%d Slot:%d", currentState, targetSlot != -1 ? targetSlot+1 : 0);
    display.print(statusLine, 1);
    // Maybe add a short delay so the message is visible
    delay(2000); 
    // Restore previous display based on current state? Requires more logic.
    changeState(currentState); // Re-trigger entry action to reset display

}


