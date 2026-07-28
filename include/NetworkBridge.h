#ifndef NETWORKBRIDGE_H
#define NETWORKBRIDGE_H

#include "../include/Config.h"
#include <Arduino.h>

class NetworkBridge {
    private:
        const char *ssid = "SINCLAIR";
        const char *password = "myPASSWORD";

    public:
        void init();
        bool isConnected();
};

#endif