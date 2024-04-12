#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "IMU.h"
#include <Arduino.h>




void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif


  
  HAL::initCSPins();
  HAL::initSensorHAL();
  // delay(1000);
  ICM::setupIMU();

    
}

void loop() {

  ICM::readIMU();

  DEBUGLN();
  delay(100);
}
