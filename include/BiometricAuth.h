#ifndef BIOMETRICAUTH_H
#define BIOMETRICAUTH_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

class BiometricAuth
{
private:
    // Pass the memory address of the ESP32's Hardware Serial 2 to the library
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

public:
    inline void init()
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
};

#endif