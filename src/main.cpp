#include <Arduino.h>
#include "../include/InputPad.h"
#include "../include/DisplayManager.h"
#include "../include/LedManager.h"
#include "../include/BiometricAuth.h"

InputPad pad;
DisplayManager display;
LedManager leds;
BiometricAuth scanner;
SystemStates currentState = IDLE_MENU;

unsigned long lastInteractionTime = 0;

void setup()
{
  Serial.begin(115200);
  pad.init();
  leds.init();
  display.init();
  scanner.init();
  display.displayMenu();
}

void loop()
{
  char pressedValue = pad.getKeyPressed();
  if (pressedValue != '\0')
  {
    lastInteractionTime = millis();

    // --- TRACK 1: TYPING THE AMOUNT ---
    if (currentState == IDLE_MENU || currentState == AWAITING_AMOUNT)
    {
      if (pressedValue == '#')
      {
        display.showAuthMenu();
        currentState = AUTH_SELECTION;
      }
      else
      {
        // If this is the very first number pressed...
        if (currentState == IDLE_MENU)
        {
          leds.setTyping();               // Turn on Yellow LED
          currentState = AWAITING_AMOUNT; // Lock into typing mode!
        }
        display.printKey(pressedValue); // Type the number
      }
    }

    else if (currentState == AUTH_SELECTION)
    {
      if (pressedValue == '1')
      {
        currentState = AWAITING_BIOMETRIC;
      }
      else if (pressedValue == '2')
      {
        display.showPasswordPrompt();
        currentState = AWAITING_PASSWORD;
      }
      else if (pressedValue == '*'){
        currentState = IDLE_MENU;
        leds.setIdle();
        display.displayMenu();
      }
    }

    else if (currentState == AWAITING_PASSWORD)
    {
      if (pressedValue == '#' && display.getEnteredPassword().length() >= MIN_PASSWORD_LENGTH)
      {

        // leds.setProcessing();

        if (display.getEnteredPassword() == MASTER_PASSWORD)
        {
          leds.setProcessing();
          delay(1000);
          display.processTransaction();
          leds.setIdle();
          currentState = IDLE_MENU;
        }
        else
        {
          leds.setError();
          display.showErrorMessage();
          delay(500);
          leds.setTyping();
          currentState = AWAITING_PASSWORD;
        }
      }
      else
      {
        display.printKey(pressedValue, true);
      }
    }
  }

  if (currentState != IDLE_MENU && millis() - lastInteractionTime >= 10000)
  {
    currentState = IDLE_MENU;
    leds.setIdle();
    display.resetInput();
  }
  delay(10);
}
