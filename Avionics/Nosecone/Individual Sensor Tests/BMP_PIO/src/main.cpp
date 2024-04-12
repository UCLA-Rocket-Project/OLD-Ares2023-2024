#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "BMP.h"
#include <Arduino.h>




void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif

  HAL::initCSPins();
  HAL::initSensorHAL();
  BMP::setupBMP();
  
    
}

void loop() {

  // Code executes if BMP is being read
  BMP::readAltimeters();

  DEBUGLN();
  delay(100);
}
