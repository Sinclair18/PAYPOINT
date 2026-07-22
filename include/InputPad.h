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

void InputPad::init()
{

    // looping through each pin on the keypad row and sending 0v to it
    for (uint8_t r = 0; r < KEYPAD_ROWS; r++)

    {
        pinMode(rowPins[r], OUTPUT);
    }


    // looping through each pin on the keypad column and listening for a press
    for (uint8_t c = 0; c < KEYPAD_COLS; c++)
    {
        pinMode(colPins[c], INPUT_PULLUP);
    }
}

char InputPad::getKeyPressed()
{

    // OUTER LOOP: The ESP32 picks one row at a time and turns it ON (LOW)
    for (uint8_t r = 0; r < KEYPAD_ROWS; r++)
    {
        digitalWrite(rowPins[r], LOW);


        // While that row r is ON, it checks every single column
        for (uint8_t c = 0; c < KEYPAD_COLS; c++) // now looping through the column, one by one for each row

        {

            if (digitalRead(colPins[c]) == LOW)
            {              // A button is pressed!
                delay(50); // Debounce

                while (digitalRead(colPins[c]) == LOW)
                    ;                           // Wait for them to let go
                digitalWrite(rowPins[r], HIGH); // Reset the row

                return keys[r][c];              // Send the character back to the FSM
            }
        }

        // so if no column was pressed in this row, it'd reset and check the next row
        digitalWrite(rowPins[r], HIGH);
    }

    
    // If we checked every single row and column and found nothing, return null character
    return '\0';
}
#endif