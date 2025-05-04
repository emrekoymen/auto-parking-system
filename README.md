# Interactive Auto‑Parking System (Arduino UNO)

## 1  Project Summary
This project implements a **sensor‑guided parking system** that detects an incoming vehicle, opens an entry barrier, rotates a platform toward an available slot, and updates visual / audio indicators in real time.

**Key goals**

* **Hands‑free guidance** in tight areas  
* **Instant feedback** via LEDs, buzzer, LCD  
* **Remote control** over Bluetooth  
* **Parking‑time logging** per slot

## 2  Hardware Bill of Materials

| Qty | Component | Purpose |
|----:|-----------|---------|
| 2 | Arduino UNO | Main controller + sensor board |
| 3 | HC‑SR04 ultrasonic sensor | Detect car in each slot |
| 2 | IR break‑beam sensor | Vehicle at entry / exit |
| 2 | Servo (SG90 / MG996R) | ① barrier  ② platform |
| 6 | LEDs (3 × red, 3 × green) | Slot status |
| 1 | 16 × 2 LCD + I²C | System messages |
| 1 | HC‑05 Bluetooth module | Remote commands |
| 1 | Piezo buzzer | Audible alert |
| 1 | DC motor + L298N | (Optional) heavy platform |
| — | Breadboard, jumpers, 220 Ω resistors, 5 V PSU | Wiring |

> *Note:* One UNO can run everything for a demo, but a second board keeps code cleaner by off‑loading slot sensors and LEDs via I²C or Serial.

## 3  System Flow

1. **Idle** – waiting for IR entry trigger  
2. **Weight / presence check** – confirm vehicle  
3. **Slot search** – ultrasonic status  
4. **Barrier logic**  
   * Free → open barrier, LCD **“Welcome”**  
   * Full → keep closed, LCD **“No Space”**  
5. **Vehicle passes IR** – close barrier  
6. **Platform rotation** – align to chosen lane  
7. **Guidance** – buzzer chirps; destination slot LED flashes green  
8. **Parked** – slot LED red, LCD updates, **timer starts**  
9. **Exit** – reverse IR triggers barrier, timer stops, LCD shows stay time, LED resets to green

See `docs/flowchart.png` for a full decision tree.

## 4  Key Features

* Automatic entry barrier with safety interlock  
* Rotating platform for angle‑restricted sites  
* Slot‑by‑slot LED status  
* 16 × 2 LCD user feedback  
* Bluetooth command interface (`START`, `ROTATE n°`, `STATUS`, …)  
* **New:** per‑slot parking duration measurement (HH:MM:SS)

## 5  Getting Started

1. **Wire up** components (see `docs/wiring.pdf`).  
2. **Clone** this repo; open `src/ParkingSystem.ino` in Arduino IDE.  
3. **Adjust pin map** in `config.h` to match your hardware.  
4. **Flash both UNOs** (if using two‑board setup).  
5. Open **Serial Monitor** or a Bluetooth terminal to view logs.

## 6  Future Extensions

* MQTT / Wi‑Fi logging to a central server  
* RFID for driver identification  
* ESP32 web dashboard  
* Computer‑vision slot detection in place of ultrasonics
