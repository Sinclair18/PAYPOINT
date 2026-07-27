// adding the header guard
#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

// included the necessary header files
#include "../include/Config.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <Arduino.h>
#include <SPI.h>

class DisplayManager
{
private:
    Adafruit_SH1106G oled{OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET};
    uint8_t inputX = 0, inputY = 0;
    String enteredAmount = "", enteredPassword = "";

public:
    // all functions to effectively handle the full FSM
    void init();
    void displayMenu();
    void printKey(char key, bool isPasswordMode);
    void showAdminMenu();
    void showAuthMenu();
    void showBiometricPrompt();
    void showPasswordPrompt();
    String getEnteredAmount();
    String getEnteredPassword();
    void processTransaction();
    void showErrorMessage(String errorMessage);
    void showSuccessMessage(String successMessage);
    void showPlaceFinger();
    void showRemoveFinger();
    void showPlaceAgain();
    void showEnrollIDPrompt();
    void showDeleteIDPrompt();
    void showDeleteAllIDPrompt();
    void resetInput();
};

inline void DisplayManager::init()
{
    oled.begin(OLED_ADDRESS, true);

    oled.clearDisplay();

    oled.setTextSize(1);
    oled.setTextColor(SH110X_WHITE, SH110X_BLACK);

    oled.display();
}

inline void DisplayManager::displayMenu()
{
    oled.clearDisplay();

    enteredAmount = "";

    oled.setTextSize(2);

    oled.setCursor(15, 0);
    oled.print("PAYPOINT");

    oled.setTextSize(1);
    oled.setCursor(5, 45);
    oled.print("Amount: ");

    oled.setCursor(41, 45);
    oled.print('$');

    inputX = 42; // Start typing after "Amt: $"
    inputY = 45; // Stay on the same row
    oled.display();
}

// adding the default parameter here!
inline void DisplayManager::printKey(char key, bool isPasswordMode = false)
{
    // 1. String Math (Same as before)
    if (key == '*')

    {
        if (isPasswordMode && enteredPassword.length() > 0)

            enteredPassword.remove(enteredPassword.length() - 1);

        else if (!isPasswordMode && enteredAmount.length() > 0)

            enteredAmount.remove(enteredAmount.length() - 1);
    }

    else

    {
        if (isPasswordMode && enteredPassword.length() < 16)

            enteredPassword += key;

        else if (!isPasswordMode && enteredAmount.length() < 16)
            enteredAmount += key;
    }

    // 2. Move the invisible cursor to our saved coordinates
    oled.setCursor(inputX, inputY);

    // 3. Draw the updated string (plus a trailing space to erase deleted characters!)
    if (isPasswordMode)

    {
        // Draw asterisks instead of the real password

        for (int i = 0; i < enteredPassword.length(); i++)
        {
            oled.print('*');
        }
        oled.print(" "); // The eraser space
    }
    else
    {
        // Draw the normal amount or ID
        oled.print(enteredAmount);
        oled.print(" "); // The eraser space
    }

    // 4. Push the update to the glass!
    oled.display();
}

inline void DisplayManager::showAdminMenu()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 11);
    oled.print("1. Enroll New ID");

    oled.setCursor(10, 31);
    oled.print("2. Delete An ID");

    oled.setCursor(10, 51);
    oled.print("3. Delete ALL ID");

    oled.display();
}

inline void DisplayManager::showAuthMenu()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("1. Biometric");

    oled.setCursor(10, 45);
    oled.print("2. Password");

    oled.display();
}

inline void DisplayManager::showBiometricPrompt()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("Place Finger");

    oled.setCursor(10, 45);
    oled.print("On Scanner...");

    oled.display();
}

inline void DisplayManager::showPasswordPrompt()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("Enter Password: ");

    inputX = 10, inputY = 45;

    oled.display();
}

inline String DisplayManager::getEnteredAmount()
{
    return enteredAmount;
}

inline String DisplayManager::getEnteredPassword()
{
    return enteredPassword;
}

inline void DisplayManager::processTransaction()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("Sending...");
    delay(1000);

    oled.clearDisplay();

    oled.setCursor(10, 45);
    oled.print("Sent: $");

    inputX = 53, inputY = 45;
    oled.print(enteredAmount); // Pulls the amount directly from memory
    oled.display();

    delay(2000);

    resetInput();
}

inline void DisplayManager::showErrorMessage(String errorMessage)
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);

    oled.print(errorMessage);

    // wiping the wrong password or entered ID value from memory so it doesn't linger
    enteredPassword = "";
    oled.display();
}

inline void DisplayManager::showSuccessMessage(String successMessage)
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);

    oled.print(successMessage);

    oled.display();
}

inline void DisplayManager::showPlaceFinger()
{
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(10, 32);
    oled.print("Place Finger");

    oled.display();
}

inline void DisplayManager::showRemoveFinger()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);
    oled.print("Remove Finger");

    oled.display();
}

inline void DisplayManager::showPlaceAgain()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);
    oled.print("Place Again");

    oled.display();
}

inline void DisplayManager::showEnrollIDPrompt()
{
    oled.clearDisplay(); // clearing the lcd totally
    enteredAmount = "";  // clearing out the admin password that was entered to get to this state

    oled.setCursor(10, 15);

    oled.print("Enter ID (1-127)");
    oled.setCursor(10, 45);
    oled.print("ID: ");

    inputX = 34, inputY = 45;
    oled.display();
}

inline void DisplayManager::showDeleteIDPrompt()
{
    enteredAmount = "";

    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("Delete ID (1-127)");

    oled.setCursor(10, 45);
    oled.print("ID: ");

    inputX = 35;
    inputY = 45;

    oled.display();
}

inline void DisplayManager::showDeleteAllIDPrompt()
{
    enteredAmount = "";
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 11);
    oled.print("DELETE ALL ID?");

    oled.setCursor(10, 31);
    oled.print("1. Delete All");

    oled.setCursor(10, 51);
    oled.print("2. Cancel");

    oled.display();
}

inline void DisplayManager::resetInput()
{
    enteredAmount = "";
    enteredPassword = "";

    displayMenu();
}

#endif