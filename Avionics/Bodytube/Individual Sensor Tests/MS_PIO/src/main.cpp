#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "MS.h"
#include <Arduino.h>




void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif

  HAL::initSensorHAL();
  MS::setupMS();
  
    
}

void loop() {

  

  MS::readAltimeter();
  delay(100);
}
