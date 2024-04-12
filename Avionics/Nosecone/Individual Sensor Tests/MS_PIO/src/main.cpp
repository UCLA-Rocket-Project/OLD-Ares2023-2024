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

  // int start = micros();
  // double p = MS::ms.getPressure();
  // DEBUGLN(micros() - start);
  
  // uint32_t start = micros();
  // MS::MS5611.read();           //  note no error checking => "optimistic".
  // uint32_t stop = micros();
  // // Serial.print("T:\t");
  // // Serial.print(MS::MS5611.getTemperature(), 2);
  // // Serial.print("\tP:\t");
  // // Serial.print(MS::MS5611.getPressure(), 2);
  // // Serial.print("\tt:\t");
  // Serial.print(stop - start);
  // Serial.println();

  MS::readAltimeter();
  delay(100);
}
