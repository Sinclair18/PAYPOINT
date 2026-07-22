/* most of the variables and macros used through out the whole codebase are initialized here
ESP32 dev module - ESP32D WROOM 38 pins dev board is what is used here*/

/* adding this header guard to avoid multiple includes of Config.h file by other header files or
cpp files*/

#ifndef CONFIG_H

#define CONFIG_H


// included to get access to arduino specific code implementation
#include <Arduino.h>

/* since I'd be using i2c lcd library, I just hard coded the lcd address, change or ignore the value if you
 choose to use another lcd type */
#define LCD_ADDRESS 0X27

/* the AS608 fingerprint module is specifically what I used for this project, the RX Pin of the
module is connected to the serial2 tx pin of esp32 dev module (pin 17) and the tx pin of the module connects
to the serial2 rx pin of the esp32 dev module*/
#define AS608_RX 17
#define AS608_TX 16



// the esp32 pins where each indicator led is connected to
#define BLUE_LED 4

#define RED_LED 15

#define YELLOW_LED 18

#define GREEN_LED 19


// hardcoding the password and minimum password length, I might later change this
const String MASTER_PASSWORD = "200518";

const int MIN_PASSWORD_LENGTH = 6;


// initializing the keypad buttons and properties that would be used in the InputPad.h file
const uint8_t KEYPAD_ROWS = 4;
const uint8_t KEYPAD_COLS = 4;

uint8_t rowPins[KEYPAD_ROWS] = {26, 27, 14, 13};
uint8_t colPins[KEYPAD_COLS] = {25, 33, 32, 23};
const char keys[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                             {'4', '5', '6', 'B'},
                                             {'7', '8', '9', 'C'},
                                             {'*', '0', '#', 'D'}};


// the FSM to handle the different modes or states of the system operation
enum SystemStates
{
    IDLE_MENU,
    AWAITING_AMOUNT,
    AUTH_SELECTION,
    AWAITING_PASSWORD,
    AWAITING_BIOMETRIC,
    PROCESSING
};

#endif