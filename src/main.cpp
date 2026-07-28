// included all the needed header files using relative path

#include <Arduino.h>
#include "../include/AppController.h"

AppController app;


void setup()
{
  // initializing all necessary components of the system
  Serial.begin(115200);

  app.init();
}

void loop()
{
  app.run();
}