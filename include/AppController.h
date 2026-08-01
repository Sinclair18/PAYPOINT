#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "../include/BiometricAuth.h"
#include "../include/Config.h"
#include "../include/DisplayManager.h"
#include "../include/InputPad.h"
#include "../include/LedManager.h"
#include "../include/BiometricAuth.h"
#include "../include/NetworkBridge.h"
#include <Preferences.h>

class AppController {
    private:
        Preferences preferences;
        BiometricAuth scanner;
        InputPad pad;
        LedManager leds;
        DisplayManager display;
        NetworkBridge router;

        SystemStates currentState = IDLE_MENU;
        unsigned long lastInteractionTime = 0;
        int failedBiometricAttempts = 0;

    public:
        void init();
        void run();
};
#endif