#include "../include/DisplayManager.h"

void DisplayManager::init()
{
    oled.begin(OLED_ADDRESS, true);

    oled.clearDisplay();

    oled.setTextSize(1);
    oled.setTextColor(SH110X_WHITE, SH110X_BLACK);

    oled.display();
}

void DisplayManager::showMainMenu()
{
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(1, 0);
    oled.print("1. Send Money");

    oled.setCursor(0, 13);
    oled.print("2. Check Balance");

    oled.setCursor(0, 26);
    oled.print("3. View History");

    oled.setCursor(0, 39);
    oled.print("4. Admin Panel");
    
    oled.setCursor(0, 52);
    oled.print("5. Manage Users");

    oled.display();
}

void DisplayManager::showAccountPrompt()
{
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(0, 10);
    oled.print("Enter Account NO: ");

    destinationAccount = "";

    oled.display();

    inputX = 0, inputY = 35;
}

void DisplayManager::showBankMenu()
{
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(0, 0);
    oled.print("Select Bank:");

    oled.setCursor(0, 12);
    oled.print("1. Access");

    oled.setCursor(0, 24);
    oled.print("2. UBA");

    oled.setCursor(0, 36);
    oled.print("3. Zenith");

    oled.setCursor(0, 48);
    oled.print("4. Test Bank");

    oled.display();
}

void DisplayManager::showConfirmation(String targetName) {
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(10, 0);
    oled.print("Send to: ");

    oled.setCursor(0, 20);
    oled.print(targetName);

    oled.setCursor(0, 50);
    oled.print("Press # to continue");

    oled.display();
}

void DisplayManager::showAmountPrompt()
{
    oled.clearDisplay();

    enteredAmount = "";

    oled.setTextSize(2);

    oled.setCursor(15, 0);
    oled.print("PAYPOINT");

    oled.setTextSize(1);
    oled.setCursor(5, 45);
    oled.print("Amount: ");

    oled.setCursor(54, 45);
    oled.print('N');
    oled.display();

    inputX = 60; // Start typing after "Amt: $"
    inputY = 45; // Stay on the same row
    oled.display();
}

// adding the default parameter here!
void DisplayManager::printKey(char key, bool isPasswordMode = false, bool isAccountMode = false)
{
    // 1. String Math (Same as before)
    if (key == '*')

    {
        if (isPasswordMode && enteredPassword.length() > 0)

            enteredPassword.remove(enteredPassword.length() - 1);

        else if (isAccountMode && destinationAccount.length() > 0)
            destinationAccount.remove(destinationAccount.length() - 1);

        else if (!isPasswordMode && !isAccountMode && enteredAmount.length() > 0)

            enteredAmount.remove(enteredAmount.length() - 1);
    }

    else

    {
        if (isPasswordMode && enteredPassword.length() < 16)

            enteredPassword += key;
            
            else if (isAccountMode && destinationAccount.length() < 10) {
                destinationAccount += key;
            }

        else if (!isPasswordMode && !isAccountMode && enteredAmount.length() < 16)
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

    else if (isAccountMode) {
        oled.print(destinationAccount);
    }

    else
    {
        // Draw the normal amount or ID
        oled.print(enteredAmount);
    }
    oled.print(" "); // The eraser space

    // 4. Push the update to the glass
    oled.display();
}

void DisplayManager::showAdminMenu()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 0);
    oled.print("1. Enroll New ID");

    oled.setCursor(10, 13);
    oled.print("2. Delete An ID");

    oled.setCursor(10, 26);
    oled.print("3. Delete ALL ID");
    
    oled.setCursor(10, 39);
    oled.print("4. Set Payment Pin");

    oled.setCursor(10, 52);
    oled.print("5. Set Admin Pin");

    oled.display();
}

void DisplayManager::showAuthMenu()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("1. Biometric");

    oled.setCursor(10, 45);
    oled.print("2. Password");

    oled.display();
}

void DisplayManager::showBiometricPrompt()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("Place Finger");

    oled.setCursor(10, 45);
    oled.print("On Scanner...");

    oled.display();
}

void DisplayManager::showPasswordPrompt()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(30, 15);
    oled.print("Enter Pin: ");

    inputX = 45, inputY = 45;

    oled.display();
}

String DisplayManager::getEnteredAmount()
{
    return enteredAmount;
}

String DisplayManager::getEnteredPassword()
{
    return enteredPassword;
}

void DisplayManager::processTransaction()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(10, 15);
    oled.print("Sending...");
    delay(1000);

    oled.clearDisplay();

    oled.setCursor(10, 45);
    oled.print("Sent: N");

    inputX = 53, inputY = 45;
    oled.print(enteredAmount); // Pulls the amount directly from memory
    oled.display();

    delay(2000);

    resetInput();
}

void DisplayManager::showErrorMessage(String errorMessage)
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);

    oled.print(errorMessage);

    // wiping the wrong password or entered ID value from memory so it doesn't linger
    enteredPassword = "";
    oled.display();
}

void DisplayManager::showSuccessMessage(String successMessage)
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(5, 20);

    oled.print(successMessage);

    enteredPassword = "";

    oled.display();
}

void DisplayManager::showPlaceFinger()
{
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(10, 32);
    oled.print("Place Finger");

    oled.display();
}

void DisplayManager::showRemoveFinger()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);
    oled.print("Remove Finger");

    oled.display();
}

void DisplayManager::showPlaceAgain()
{
    oled.clearDisplay();

    oled.setTextSize(1);

    oled.setCursor(20, 32);
    oled.print("Place Again");

    oled.display();
}

void DisplayManager::showEnrollIDPrompt()
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

void DisplayManager::showSetPinPrompt(String pinName){
    enteredPassword = "";
    oled.clearDisplay();
    oled.setTextSize(1);

    oled.setCursor(0, 10);
    oled.print("New " + pinName + ": ");
    oled.display();

    oled.setCursor(20, 35);
}

void DisplayManager::showDeleteIDPrompt()
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

void DisplayManager::showDeleteAllIDPrompt()
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

void DisplayManager::resetInput()
{
    enteredAmount = "";
    enteredPassword = "";
    destinationAccount = "";

    showMainMenu();
}