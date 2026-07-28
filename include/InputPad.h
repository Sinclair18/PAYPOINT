// header guards
#ifndef INPUTPAD_H
#define INPUTPAD_H

// added to access the builtin methods, functions and variables from the Config.h and the Arduino.h files
#include <Arduino.h>
#include "Config.h"


// the keypad class to use the keypad properties that was declared in the Config.h file
class InputPad
{
private:
// to track the last key pressed so that the main.cpp knows how to parse it when checking for the 10 seconds of inactivity
    char lastKeyPressed;

public:
    void init();

    char getKeyPressed();
};

#endif