#ifndef BARRIER_H
#define BARRIER_H

#include "../config.h"
#include <Arduino.h>
#include <Servo.h> // Include the Servo library

class Barrier {
public:
    void setup();
    void open();
    void close();

private:
    Servo _servo;    // Servo object
    bool _isOpen = false;
};

#endif // BARRIER_H
