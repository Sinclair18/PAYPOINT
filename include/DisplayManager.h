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
    void showMainMenu();
    void showAmountPrompt();
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


#endif