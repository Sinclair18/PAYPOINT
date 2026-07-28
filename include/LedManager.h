#ifndef LEDMANAGER_H
#define LEDMANAGER_H


#include "config.h"

class LedManager{
    public:
        void init();

        void setIdle();

        void setTyping();

        void setProcessing();
        void setError();
};


#endif