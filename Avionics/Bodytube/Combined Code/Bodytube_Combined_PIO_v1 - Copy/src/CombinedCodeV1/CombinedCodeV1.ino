// #define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "MS.h"
#include "Radio.h"
#include "XTSD.h"
#include "INA.h"
#include <Arduino.h>


int lastTransmissionTime = 0;

void radioGPS(void* param) {
  while(1) {

    if (millis() - Radio::lastTransmissionTime > 100) {

      Radio::downlink_packet = micros();
      Radio::downlink_packet += "," + String(MS::temp, 2) + "," + String(MS::pressure, 2) + "," + String(MS::altitude, 2) + ",";
      Radio::downlink_packet += String(INA::bus_voltage, 2) + "," + XTSD::logSuccess;
  
      int oldTime = micros();
      Radio::radio.beginPacket();
      Radio::radio.print(Radio::downlink_packet);
      Radio::radio.endPacket();
      Radio::lastTransmissionTime = micros() - oldTime;  
      DEBUG("transmit with transmission time: ");
      DEBUGLN(Radio::lastTransmissionTime);
      
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef DEBUG_MODE
    while(!Serial);   
  #endif

  INA::setupINA();

  HAL::initCSPins();
  HAL::initSensorHAL();
  MS::setupMS();
  XTSD::setupSD();

  HAL::initRadioHAL();

  Radio::setupRadio();

  xTaskCreatePinnedToCore (
    radioGPS,     // Function to implement the task
    "radioGPS",   // Name of the task
    10000,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );
}

void loop() {

  MS::readAltimeter();

  
  INA::readINA();

  

  int oldLog = micros();
  XTSD::logStr = (String)micros();
  XTSD::logStr += "," + String(XTSD::logTime) + "," + String(MS::temp, 2) + "," + String(MS::pressure, 2) + "," + String(MS::altitude, 2) + ",";
  XTSD::logStr += String(INA::bus_voltage);
  XTSD::logSD(XTSD::logStr);
  XTSD::logTime = micros() - oldLog;
  delay(10);
}


