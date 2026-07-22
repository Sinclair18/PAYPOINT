// included all the needed header files using relative path

#include <Arduino.h>

#include "../include/InputPad.h"
#include "../include/DisplayManager.h"
#include "../include/LedManager.h"
#include "../include/BiometricAuth.h"


/* created all the necessary instances of their respective classes as declared in 
their respective header files to keep things modular*/
InputPad pad;

DisplayManager display;

LedManager leds;

BiometricAuth scanner;

SystemStates currentState = IDLE_MENU;


// the timer to reset the system back to idle after 10 seconds of not pressing anything
unsigned long lastInteractionTime = 0;

void setup()
{
  // initializing all necessary components of the system
  Serial.begin(115200);
  pad.init();

  leds.init();

  display.init();

  scanner.init();

  display.displayMenu();
}

void loop()
{

  /*the variable that'd store the return value of the getKeyPressed() function that
  I declared in the inputPad class*/
  char pressedValue = pad.getKeyPressed();

  /* if any key at all is pressed, we start the timer and save the current time to 
  the lastInteractionTime variable
  
  */
  if (pressedValue != '\0')

  {

    lastInteractionTime = millis();

    /* *track 1 - following the FSM that I defined in the Config.h file, if the current state is idle
    or waiting for an amount to be entered, and the hashtag value is entered, the current state
    changes to the auth auth selection state with two sub states - Biometric and Password */


    if (currentState == IDLE_MENU || currentState == AWAITING_AMOUNT)
    {

      if (pressedValue == '#')

      {
        display.showAuthMenu();
        currentState = AUTH_SELECTION;
      }
      /* or else if the current state is specifically in the idle menu and the hashtag value 
      is entered, the led activates for set typing*/

      else
      {
        // If this is the very first number pressed...
        if (currentState == IDLE_MENU)

        {

          leds.setTyping();               // Turn on Yellow LED
          currentState = AWAITING_AMOUNT; // Lock into typing mode

        }

        display.printKey(pressedValue); // Type the number
      }

    }
     
    /* *track 2 - if the current state is in the auth selection, there'd be two sub choices, the Biometric
    and the Password*/
    else if (currentState == AUTH_SELECTION)
    {
      /* if the choice is the biometric, the current states changes to
      awaiting biometric*/
      if (pressedValue == '1')
      {

        currentState = AWAITING_BIOMETRIC;

      }

      // or else if 2 is selected, the current state changes to awaiting password
      else if (pressedValue == '2')

      {
        display.showPasswordPrompt();
        currentState = AWAITING_PASSWORD;
      }
      /* but if the '*' is entered which represents "back", the system FSM moves back one state,
      which goes back to idle state, and other idle functions from input pad and display manager
      headers are called*/

      else if (pressedValue == '*'){

        currentState = IDLE_MENU;
        leds.setIdle();

        display.displayMenu();
      }
    }

    /* track 3 if the current state is awaiting password and the password entered matches the 
    one defined in the config.h file and is also greater than or equal to the minimum that should 
    be entered, the money gets transferred and the system goes back to idle*/
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

        /* but if the password is wrong, the system logs an error and gives another chance to re-input
        the password (I would later add a maximum amount of trials before the system gets locked)*/

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

  /* back to that timer that was set to watch out for 10 seconds of inactivity, the system resets
  to idle if the condition is met*/
  if (currentState != IDLE_MENU && millis() - lastInteractionTime >= 10000)
  {
    currentState = IDLE_MENU;
    leds.setIdle();
    display.resetInput();
  }
  delay(10); // tiny delay for smoothing the esp32
}
