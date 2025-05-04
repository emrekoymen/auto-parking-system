#ifndef BLUETOOTH_CMD_H
#define BLUETOOTH_CMD_H

#include "../config.h"
#include <Arduino.h>

// Forward declare main sketch functions or use function pointers/callbacks
// Example callbacks (adjust signature as needed):
typedef void (*RotateCmdCallback)(int angle);
typedef void (*StatusCmdCallback)();
// Add more callback types for other commands (START, etc.)

class BluetoothCmd {
public:
    // Pass function pointers during setup
    void setup(RotateCmdCallback rotateCallback, StatusCmdCallback statusCallback);
    
    // Call this regularly in the main loop to check for incoming commands
    void checkCommands(); 

private:
    String _commandBuffer = ""; // Buffer for incoming serial data
    const char COMMAND_TERMINATOR = '\n'; // Character indicating end of command

    // Pointers to callback functions in the main sketch
    RotateCmdCallback _onRotateCmd = nullptr;
    StatusCmdCallback _onStatusCmd = nullptr;

    // Function to process a complete command line
    void processCommand(String commandLine);
};

#endif // BLUETOOTH_CMD_H
