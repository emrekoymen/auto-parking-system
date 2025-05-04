# Wiring Guide (Main Controller - uno_main)

This guide outlines the connections for the primary Arduino UNO controller based on `src/config.h`. Ensure your 5V power supply is adequate, especially for servos. **Always disconnect power before making wiring changes.**

**Reference `src/config.h` for exact pin numbers.**

## 1. Power Distribution

*   Connect the **5V output** from your power supply unit (PSU) to the breadboard's positive (+) rail.
*   Connect the **GND** from your PSU to the breadboard's negative (-) rail.
*   Connect the Arduino **5V pin** to the positive rail.
*   Connect the Arduino **GND pin** to the negative rail.

## 2. Barrier Servo (SG90 / MG996R)

*   **Signal Pin:** Connect to Arduino pin `PIN_BARRIER_SERVO` (Default: `D9`).
*   **VCC Pin:** Connect to the breadboard's **+5V rail**.
*   **GND Pin:** Connect to the breadboard's **GND rail**.

## 3. Platform Servo (SG90 / MG996R)

*   **Signal Pin:** Connect to Arduino pin `PIN_PLATFORM_SERVO` (Default: `D10`).
*   **VCC Pin:** Connect to the breadboard's **+5V rail**.
*   **GND Pin:** Connect to the breadboard's **GND rail**.
    *   *Note:* If using a DC motor + L298N driver instead, consult the L298N datasheet for connections (IN pins to Arduino, motor power to PSU). The `PIN_PLATFORM_SERVO` might be repurposed for direction or speed control.

## 4. Ultrasonic Sensors (HC-SR04 x NUM_SLOTS)

*   Connect **VCC** pin of each sensor to the **+5V rail**.
*   Connect **GND** pin of each sensor to the **GND rail**.
*   For **Slot 0**:
    *   `Trig` Pin -> Arduino `PINS_TRIG[0]` (Default: `D2`)
    *   `Echo` Pin -> Arduino `PINS_ECHO[0]` (Default: `D3`)
*   For **Slot 1**:
    *   `Trig` Pin -> Arduino `PINS_TRIG[1]` (Default: `D4`)
    *   `Echo` Pin -> Arduino `PINS_ECHO[1]` (Default: `D5`)
*   For **Slot 2**:
    *   `Trig` Pin -> Arduino `PINS_TRIG[2]` (Default: `D7`)
    *   `Echo` Pin -> Arduino `PINS_ECHO[2]` (Default: `D8`)

## 5. IR Break-Beam Sensors (x2)

*   These typically have 3 pins: VCC, GND, OUT.
*   Connect **VCC** pin of each sensor to the **+5V rail**.
*   Connect **GND** pin of each sensor to the **GND rail**.
*   **Entry Sensor OUT Pin:** Connect to Arduino `PIN_IR_ENTRY` (Default: `A0`).
*   **Exit Sensor OUT Pin:** Connect to Arduino `PIN_IR_EXIT` (Default: `A1`).
    *   *Note:* `config.h` uses `INPUT_PULLUP`, assuming the sensor outputs LOW when the beam is broken. Verify your sensor's behavior. If it outputs HIGH when broken, connect a pull-down resistor (e.g., 10kΩ) from the OUT pin to GND instead of using the internal pull-up.

## 6. Slot Status LEDs (Green/Red x NUM_SLOTS)

*   For each slot `i` (0 to `NUM_SLOTS - 1`):
    *   **Green LED:**
        *   Connect the **Anode** (longer leg) to Arduino pin `PINS_LED_GREEN[i]` (Defaults: `D11`, `D12`, `D13`).
        *   Connect the **Cathode** (shorter leg) through a **220Ω resistor** to the **GND rail**.
    *   **Red LED:**
        *   Connect the **Anode** (longer leg) to Arduino pin `PINS_LED_RED[i]` (Defaults: `A2`, `A3`, `A4`).
        *   Connect the **Cathode** (shorter leg) through a **220Ω resistor** to the **GND rail**.

## 7. Piezo Buzzer

*   Connect one pin to Arduino `PIN_BUZZER` (Default: `D6`).
*   Connect the other pin to the **GND rail**. (Polarity usually doesn't matter unless it's an active buzzer with marked pins).

## 8. LCD (16x2 I2C)

*   Connect **VCC** pin to the **+5V rail**.
*   Connect **GND** pin to the **GND rail**.
*   Connect **SDA** pin to Arduino **SDA pin** (Usually `A4` on UNO).
*   Connect **SCL** pin to Arduino **SCL pin** (Usually `A5` on UNO).
    *   *Note:* Verify the I2C address (`LCD_ADDR` in `config.h`, Default: `0x27`) matches your specific module.

## 9. Bluetooth Module (HC-05)

*   Connect **VCC** pin to the **+5V rail** (or 3.3V, check your module's requirements).
*   Connect **GND** pin to the **GND rail**.
*   Connect **TXD** (Transmit) pin to Arduino **RX pin** (Default: `D0` - Hardware Serial `Serial`).
*   Connect **RXD** (Receive) pin to Arduino **TX pin** (Default: `D1` - Hardware Serial `Serial`).
    *   *Level Shifting:* If the HC-05 operates at 3.3V logic and the Arduino is 5V, a level shifter (or voltage divider on the Arduino TX -> HC-05 RX line) is recommended.
    *   *Note:* Using pins `D0`/`D1` prevents using the USB Serial Monitor simultaneously for debugging while Bluetooth is connected. Consider using SoftwareSerial on different pins if needed, and update `config.h` and `BluetoothCmd.cpp` accordingly.

## 10. (Optional) Second Arduino (Sensor Board)

*   If using the two-board I2C setup:
    *   Connect **GND** of Sensor board to **GND** of Main Controller board.
    *   Connect **SDA** (A4) of Sensor board to **SDA** (A4) of Main Controller.
    *   Connect **SCL** (A5) of Sensor board to **SCL** (A5) of Main Controller.
    *   Power the Sensor board separately or from the main 5V rail.
    *   (Optional) Add pull-up resistors (e.g., 4.7kΩ) from SDA to 5V and SCL to 5V, placed once on the bus.
    *   Slot sensors and LEDs would connect to the *Sensor board* Arduino instead of the main one. The wiring would follow similar principles but use the pins defined in the `uno_sensor` environment's configuration. 