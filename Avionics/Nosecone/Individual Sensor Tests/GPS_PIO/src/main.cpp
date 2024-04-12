#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "GPS.h"
#include <Arduino.h>




void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif
  
  GPS::setupGPS();
    
}

void loop() {

  GPS::readGPS();

  DEBUGLN();
  delay(100);
}
