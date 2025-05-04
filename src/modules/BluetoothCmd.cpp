#include "BluetoothCmd.h"

// Setup: Initialize Serial and store callbacks
void BluetoothCmd::setup(RotateCmdCallback rotateCallback, StatusCmdCallback statusCallback) {
    // Assuming HC-05 default baud rate is often 9600, but might be 38400 or other.
    // Make sure this matches the HC-05 configuration AND monitor_speed in platformio.ini if debugging.
    // Note: Using Hardware Serial (Serial) conflicts with USB programming/Serial Monitor
    // unless you disconnect the HC-05 during upload or use SoftwareSerial.
    Serial.begin(SERIAL_BAUD_RATE); // Use baud rate from config.h
    _commandBuffer.reserve(64); // Pre-allocate buffer space
    _onRotateCmd = rotateCallback;
    _onStatusCmd = statusCallback;
    // Serial.println("BluetoothCmd setup complete. Ready for commands...");
}

// Check for incoming commands from Serial
void BluetoothCmd::checkCommands() {
    while (Serial.available() > 0) {
        char receivedChar = Serial.read();

        if (receivedChar == COMMAND_TERMINATOR) {
            // End of command reached, process it
            _commandBuffer.trim(); // Remove leading/trailing whitespace
            if (_commandBuffer.length() > 0) {
                // Serial.print("Received command: "); Serial.println(_commandBuffer);
                processCommand(_commandBuffer);
            }
            _commandBuffer = ""; // Clear buffer for next command
        } else if (isPrintable(receivedChar)) {
            // Append printable characters to the buffer
            if (_commandBuffer.length() < 63) { // Prevent buffer overflow
                 _commandBuffer += receivedChar;
            }
        } 
        // Ignore non-printable characters (except terminator)
    }
}

// Parse and act on a received command line
void BluetoothCmd::processCommand(String commandLine) {
    // Convert command to uppercase for case-insensitive comparison
    commandLine.toUpperCase(); 

    if (commandLine.startsWith("ROTATE ")) {
        // Extract angle value
        String angleStr = commandLine.substring(7); // Get text after "ROTATE "
        angleStr.trim();
        int angle = angleStr.toInt(); // Convert to integer

        // Check if conversion was successful (toInt returns 0 on failure)
        // and if a callback is registered
        if ((angle != 0 || angleStr == "0") && _onRotateCmd) { 
             _onRotateCmd(angle);
        } else {
            // Serial.println("Error: Invalid angle for ROTATE command.");
        }
    } else if (commandLine == "STATUS") {
        if (_onStatusCmd) {
            _onStatusCmd();
        }
    } else if (commandLine == "START") {
        // Add handling for START command if needed (e.g., trigger entry)
        // if (_onStartCmd) _onStartCmd();
        // Serial.println("Received START command (handler not implemented)");
    } 
    // Add more else if blocks for other commands (e.g., OPEN, CLOSE, RESET)
    else {
        // Serial.print("Unknown command: "); Serial.println(commandLine);
    }
}
