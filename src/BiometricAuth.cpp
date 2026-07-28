#include "../include/BiometricAuth.h"

void BiometricAuth::init()
{
    // booting up the serial2 pins and the baudrate to 57600 bits per second
    Serial2.begin(57600, SERIAL_8N1, 16, 17);

    // now telling the sensor library to start listening
    finger.begin(57600);

    // confirm that the sensor is alive
    if (finger.verifyPassword())
    {
        Serial.println("AS608 Sensor connected successfully!");
    }
    else
    {
        Serial.println("ERROR: Could not find fingerprint sensor. Check wiring!");
    }
}

int BiometricAuth::scanForMatch()
{
    uint8_t p = finger.getImage();

    if (p == FINGERPRINT_NOFINGER)
    {
        return 0;
    }

    if (p != FINGERPRINT_OK)
    {
        return 2;
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
    {
        return 2;
    }

    p = finger.fingerSearch();

    if (p == FINGERPRINT_OK)
    {
        return 1;
    }

    return 2;
}

bool BiometricAuth::enrollFingerprint(uint8_t id, DisplayManager &display)
{
    int p = -1;
    unsigned long startTime = millis();

    // 1. Wait for the user to place their finger for the first time
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        delay(50);
        if (millis() - startTime > 10000)
            return false;
    }

    p = finger.image2Tz(1);
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.fingerFastSearch();

    if (p == FINGERPRINT_OK)
    {
        display.showErrorMessage("Duplicate Detected");
        return false;
    }

    // --- NEW: Tell the user to lift their finger! ---
    display.showRemoveFinger();

    // 2. Wait for the user to remove their finger
    p = 0;
    startTime = millis();
    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger.getImage();
        delay(50);
        if (millis() - startTime > 10000)
            return false;
    }

    // --- NEW: Tell the user to scan it again! ---
    display.showPlaceAgain();

    // 3. Wait for them to place the same finger again
    p = -1;
    startTime = millis();
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        delay(50);
        if (millis() - startTime > 10000)
            return false;
    }

    p = finger.image2Tz(2);
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.createModel();
    if (p != FINGERPRINT_OK)
        return false;

    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
        return true;

    return false;
}

bool BiometricAuth::deleteFingerprint(uint8_t id)
{
    uint8_t p = finger.loadModel(id);

    if (p != FINGERPRINT_OK)
    {
        return false; // it doesn't exist
    }

    p = finger.deleteModel(id);

    if (p == FINGERPRINT_OK)

    {
        return true;
    }

    return false; // Failed to delete (or ID didn't exist)
}

bool BiometricAuth::deleteAllFingerprints()
{
    uint8_t p = finger.emptyDatabase();
    Serial.print("Database Clear Code: ");
    Serial.println(p);

    if (p == FINGERPRINT_OK)
    {
        return true;
    }

    return false;
}

bool BiometricAuth::isIDTaken(uint8_t id)
{
    uint8_t p = finger.loadModel(id);
    if (p == FINGERPRINT_OK)
    {
        return true;
    }

    return false;
}
