#include "../include/AppController.h"

void AppController::init()
{
    Serial.begin(115200);

    pad.init();

    leds.init();

    display.init();

    scanner.init();

    display.displayMenu();
}

void AppController::run()
{
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

                if (display.getEnteredAmount() == ADMIN_PASSWORD)
                {

                    currentState = ADMIN_MODE;
                    display.showAdminMenu();
                }

                else
                {

                    display.showAuthMenu();
                    currentState = AUTH_SELECTION;
                }
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
                display.showBiometricPrompt();
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

            else if (pressedValue == '*')
            {

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
            if (pressedValue == '#')
            {
                // 1. First, check if the password is long enough to even bother verifying
                if (display.getEnteredPassword().length() < MIN_PASSWORD_LENGTH)
                {
                    leds.setError();
                    display.showErrorMessage("Wrong Password");
                    delay(500);
                    leds.setTyping();
                    currentState = AWAITING_PASSWORD;
                }
                // 2. If it is long enough, do the actual master password check
                else if (display.getEnteredPassword() == MASTER_PASSWORD)
                {
                    leds.setProcessing();
                    delay(1000);
                    display.processTransaction();
                    leds.setIdle();
                    currentState = IDLE_MENU;
                }
                // 3. If it's long enough but just wrong
                else
                {
                    leds.setError();
                    display.showErrorMessage("Wrong Password");
                    delay(500);
                    leds.setTyping();
                    currentState = AWAITING_PASSWORD;
                }
            }
            // If it wasn't the # key, just type the character
            else
            {
                display.printKey(pressedValue, true);
            }
        }
        else if (currentState == AWAITING_BIOMETRIC)
        {
            int matchResult = scanner.scanForMatch();

            if (pressedValue == '*')
            {
                leds.setIdle();
                currentState = IDLE_MENU;
                display.resetInput();
            }

            if (matchResult == 1) // Match Found!
            {
                leds.setProcessing();
                delay(600);
                display.processTransaction();
                leds.setIdle();
                currentState = IDLE_MENU;
                lastInteractionTime = millis();
            }

            else if (matchResult == 2) // Wrong Finger!
            {
                leds.setError();
                display.showErrorMessage("Invalid Finger.");

                delay(1500); // Give them time to read the error
                leds.setIdle();

                // Redraw the biometric prompt so they can try again before the 10-second timer runs out
                display.showBiometricPrompt();
                lastInteractionTime = millis(); // Reset the inactivity timer to give them a fair second chance
            }
        }

        else if (currentState == ADMIN_MODE)
        {
            if (pressedValue == '*')
            {
                // escape out to the main menu
                currentState = IDLE_MENU;
                leds.setIdle();
                display.resetInput();
            }

            else if (pressedValue == '1')
            {
                leds.setProcessing();
                delay(500);
                leds.setIdle();
                // the "Start Enrollment" process then starts
                display.showEnrollIDPrompt();
                currentState = AWAITING_ENROLL_ID;
            }

            else if (pressedValue == '2')
            {
                // the "delete ID" process starts here
                leds.setProcessing();
                delay(500);
                leds.setIdle();

                display.showDeleteIDPrompt();
                currentState = AWAITING_DELETE_ID;
            }

            else if (pressedValue == '3')
            {
                // the "delete All ID" process starts here
                leds.setProcessing();
                delay(500);
                leds.setIdle();

                display.showDeleteAllIDPrompt();
                currentState = AWAITING_DELETE_ALL_ID;
            }
        }

        else if (currentState == AWAITING_ENROLL_ID)
        {
            if (pressedValue == '*')
            {
                display.showAdminMenu();
                currentState = ADMIN_MODE;
            }

            else if (pressedValue >= '0' && pressedValue <= '9')
            {
                display.printKey(pressedValue, false);
            }

            else if (pressedValue == '#')
            {
                int enrollID = display.getEnteredAmount().toInt();

                if (enrollID >= 0 && enrollID <= 127)
                {
                    //  Check if the slot is already taken
                    if (scanner.isIDTaken(enrollID) == true)
                    {
                        leds.setError();
                        display.showErrorMessage("ID Already Taken");
                        delay(1500);
                        leds.setIdle();

                        // Kick them back to the admin menu
                        display.showAdminMenu();
                        currentState = ADMIN_MODE;
                    }
                    else
                    {
                        // The slot is free! Proceed with the scanning process
                        display.showPlaceFinger();

                        if (scanner.enrollFingerprint(enrollID, display) == true)
                        {
                            leds.setProcessing();
                            display.showSuccessMessage("Enroll Success!");
                            delay(1500);
                            leds.setIdle();
                        }
                        else
                        {
                            leds.setError();
                            display.showErrorMessage("ID in Use");
                            delay(1500);
                            leds.setIdle();
                        }

                        display.showAdminMenu();
                        currentState = ADMIN_MODE;
                    }
                }
            }
        }

        else if (currentState == AWAITING_DELETE_ID)
        {

            if (pressedValue == '*')
            {
                display.showAdminMenu();
                currentState = ADMIN_MODE;
            }

            else if (pressedValue >= '0' && pressedValue <= '9')
            {
                display.printKey(pressedValue, false);
            }

            else if (pressedValue == '#')

            {
                int IDToDelete = display.getEnteredAmount().toInt();

                if (IDToDelete >= 0 && IDToDelete <= 127)
                {

                    if (scanner.deleteFingerprint(IDToDelete) == true)

                    {

                        display.showSuccessMessage("ID has been removed.");

                        leds.setProcessing();
                        delay(1500);

                        leds.setIdle();
                    }

                    else
                    {
                        leds.setError();

                        display.showErrorMessage("ID does not exist.");
                        delay(1500);
                        leds.setIdle();
                    }

                    display.showAdminMenu();
                    currentState = ADMIN_MODE;
                }
            }
        }

        else if (currentState == AWAITING_DELETE_ALL_ID)
        {

            if (pressedValue == '1')
            {

                if (scanner.deleteAllFingerprints() == true)
                {

                    leds.setProcessing();
                    display.showSuccessMessage("All ID Cleared.");

                    delay(1000);

                    leds.setIdle();
                }

                else
                {
                    leds.setError();
                    display.showErrorMessage("NO ID TO CLEAR.");

                    delay(1000);

                    leds.setIdle();
                }
                display.showAdminMenu();
                currentState = ADMIN_MODE;
            }

            else if (pressedValue == '2')
            {
                display.showAdminMenu();
                currentState = ADMIN_MODE;
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

    if (currentState == AWAITING_BIOMETRIC)
    {

        if (scanner.scanForMatch() == true)

        {

            leds.setProcessing();

            delay(600);

            display.processTransaction();
            leds.setIdle();

            currentState = IDLE_MENU;
            lastInteractionTime = millis();
        }
    }
}