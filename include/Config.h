#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h> // included to get access to arduino specific code implementation


// defining the lcd
#define LCD_ADDRESS 0X27

// the AS608 fingerprint module pin connection
#define AS608_RX 17
#define AS608_TX 16

// the indicator leds
#define BLUE_LED 4
#define RED_LED 15
#define YELLOW_LED 18
#define GREEN_LED 19

// setting the password value
const String MASTER_PASSWORD = "200518";
const int MIN_PASSWORD_LENGTH = 6;

// keypad pins
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

// defining the 16 bit hex value colors for the screen, they are RGB565 not the regular RGB codes
const uint16_t COLOR_PAYPOINT_GREEN = 0x07E0;
const uint16_t COLOR_PAYPOINT_BLACK = 0X0000;
const uint16_t COLOR_PAYPOINT_WHITE = 0x1111;
#endif