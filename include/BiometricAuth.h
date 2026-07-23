/* adding this header guard to avoid multiple includes of BiometricAuth.h file by other header files or
cpp files*/

#ifndef BIOMETRICAUTH_H
#define BIOMETRICAUTH_H

#include <Arduino.h>

/* adding the adafruit fingerprint library to access the usable functions in the library for
scanning user fingerprint*/
#include <Adafruit_Fingerprint.h>


/* the class created to initialize the fingerprint module and put the authentication functions
to so I don't flood the main.cpp with them*/
class BiometricAuth
{
private:
    // Pass the memory address of the ESP32's Hardware Serial 2 to the library
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

public:
    void init();
};


inline void BiometricAuth::init() 
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

#endif