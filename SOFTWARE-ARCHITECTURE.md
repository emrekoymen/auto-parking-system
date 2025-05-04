# Software Architecture

src/
├── ParkingSystem.ino // main state‑machine loop
├── modules/
│ ├── Barrier.h/.cpp // open/close logic + debounce
│ ├── SlotSensor.h/.cpp // ultrasonic read, median filter
│ ├── Platform.h/.cpp // rotatePlatformToDirection()
│ ├── Display.h/.cpp // LCD + LED abstractions
│ ├── BluetoothCmd.h/.cpp // parse HC‑05 packets
│ └── Timer.h/.cpp // parking‑time tracker
└── config.h // pin map, thresholds, slot count


## State Machine

| State | Entry Action | Exit Condition | Next |
|-------|--------------|----------------|------|
| **IDLE** | LCD “Waiting” | IR entry LOW→HIGH | `WEIGHT_CHECK` |
| **WEIGHT_CHECK** | `checkVehicleWeight()` | slot free → `BARRIER_OPEN`  /  full → `FULL` |
| **BARRIER_OPEN** | open servo, LCD “Welcome” | IR entry HIGH→LOW | `PLATFORM_ALIGN` |
| **PLATFORM_ALIGN** | `rotatePlatformToDirection()` | rotation done | `GUIDE` |
| **GUIDE** | buzzer on/off, LEDs flash | `SlotSensor.triggered()` | `PARKED` |
| **PARKED** | `Timer.start(slot)`; LED red | IR exit LOW→HIGH | `EXIT` |
| **EXIT** | open barrier, `Timer.stop(slot)` | IR exit HIGH→LOW | `IDLE` |
| **FULL** | LCD “No Space” | timeout or slot free | `IDLE` |

## Core APIs

```cpp
// Timer.h
struct ParkingTimer {
  unsigned long startTs[NUM_SLOTS] = {0};
  unsigned long duration[NUM_SLOTS] = {0};

  void start(uint8_t slot) { startTs[slot] = millis(); }

  void stop(uint8_t slot) {
    duration[slot] = millis() - startTs[slot];
    logToLCD(slot, duration[slot]);  // show HH:MM:SS
  }

  unsigned long getSeconds(uint8_t slot) {
    return duration[slot] / 1000;
  }
};
Barrier::open() / Barrier::close() – servo pulses, safety delay

SlotSensor::isFree(i) – median of 5 ultrasonic reads

Display::setSlotLED(i, GREEN | RED)

BluetoothCmd::onReceive() – callbacks for ROTATE <deg> etc.

Inter‑Board Communication (optional two‑board setup)
I²C Master – main ParkingSystem UNO

I²C Slave (0x28) – sensor board returns 1‑byte slot bitmask

Build & Deployment (PlatformIO)
platformio run -e uno_main        # build main controller
platformio run -e uno_sensor      # build sensor board
platformio device monitor -e uno_main
