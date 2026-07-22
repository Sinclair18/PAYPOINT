#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "../include/Config.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <SPI.h>

class DisplayManager
{
private:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(LCD_ADDRESS, 16, 2);
    int cursorPosition = 7;
    String enteredAmount = "";
    String enteredPassword = "";

public:
    void init();
    void displayMenu();
    void printKey(char key, bool isPasswordMode);
    void resetInput();
    void processTransaction();
    void showAuthMenu();
    void showPasswordPrompt();
    String getEnteredPassword();
    void showErrorMessage();
};

inline void DisplayManager::init()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

inline void DisplayManager::displayMenu()
{
    enteredAmount = "";
    cursorPosition = 7;
    lcd.clear();

    lcd.setCursor(4, 0);
    lcd.print("PAYPOINT");

    lcd.setCursor(0, 1);
    lcd.print("Amt: $");
}

// adding the default parameter here!
inline void DisplayManager::printKey(char key, bool isPasswordMode = false)
{
    // first the backspace logic
    if (key == '*')
    {
        // now if we are in password mode, we just delete from the password string
        if (isPasswordMode && enteredPassword.length() > 0)
        {
            enteredPassword.remove(enteredPassword.length() - 1);
            cursorPosition -= 1;
            lcd.setCursor(cursorPosition, 1);
            lcd.print(" ");
        }
        // but if are in normal mode, delete from the amount string
        else if (!isPasswordMode && enteredAmount.length() > 0)
        {
            enteredAmount.remove(enteredAmount.length() - 1);
            cursorPosition -= 1;
            lcd.setCursor(cursorPosition, 1);
            lcd.print(" ");
        }
    }
    // now for the typing logic
    else
    {
        if (cursorPosition < 16)
        {
            lcd.setCursor(cursorPosition, 1);

            // If it is a password, save the real key but print an asterisk!
            if (isPasswordMode)
            {
                enteredPassword += key;
                lcd.print('*');
            }
            // Otherwise, act completely normal
            else
            {
                enteredAmount += key;
                lcd.print(key);
            }
            cursorPosition += 1;
        }
    }
}

inline void DisplayManager::resetInput()
{
    enteredAmount = "";
    enteredPassword = "";
    cursorPosition = 7;

    lcd.clear();
    displayMenu();
}

inline void DisplayManager::processTransaction()
{
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Sending...");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sent: $");
    lcd.print(enteredAmount); // Pulls the amount directly from memory
    delay(2000);

    resetInput();
}

inline void DisplayManager::showAuthMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("1. Biometric");

    lcd.setCursor(0, 1);
    lcd.print("2. Password");
}

inline void DisplayManager::showPasswordPrompt()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password: ");

    cursorPosition = 5;
}

inline String DisplayManager::getEnteredPassword(){
    return enteredPassword;
}

inline void DisplayManager::showErrorMessage()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Wrong Password");
    delay(2000); // Hold the message on screen for 2 seconds

    // Wipe the wrong password from memory so it doesn't linger
    enteredPassword = "";

    // Redraw the default PayPoint menu
    showPasswordPrompt();
}
#endif
