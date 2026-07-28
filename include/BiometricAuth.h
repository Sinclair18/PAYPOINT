/* adding this header guard to avoid multiple includes of BiometricAuth.h file by other header files or
cpp files*/

#ifndef BIOMETRICAUTH_H
#define BIOMETRICAUTH_H

#include <Arduino.h>

/* adding the adafruit fingerprint library to access the usable functions in the library for
scanning user fingerprint*/
#include <Adafruit_Fingerprint.h>

#include "../include/DisplayManager.h"

/* the class created to initialize the fingerprint module and put the authentication functions
to so I don't flood the main.cpp with them*/
class BiometricAuth
{
private:
    // Pass the memory address of the ESP32's Hardware Serial 2 to the library
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

public:
    void init();
    int scanForMatch();
    bool enrollFingerprint(uint8_t id, DisplayManager &display);
    bool deleteFingerprint(uint8_t id);
    bool deleteAllFingerprints();
    bool isIDTaken(uint8_t id);
    
};

#endif