#ifndef NETWORKBRIDGE_H
#define NETWORKBRIDGE_H

#include "../include/Config.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Arduino.h>

class NetworkBridge {
    private:
        const char *ssid = "SINCLAIR";
        const char *password = "myPASSWORD";

        const char *paystackKey = "Bearer sk_test_fc6ba6ffd558ef1b5cf8b8f7477dca06b0cdad57";

    public:
        void init();
        bool isConnected();

        void sendTransaction(int amount, String authMethod, int userID);
        void testPaystackName();
};

#endif