#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "BMP.h"
#include "GPS.h"
#include "ICM.h"
#include "Radio.h"
#include <Arduino.h>




void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif

  HAL::initSensorHAL();

  BMP::setupBMP();
  ICM::setupICM();
  GPS::setupGPS();

  HAL::initRadioHAL();
  // Radio::setupRadio();

}

void loop() {

  // Code executes if BMP is being read
  BMP::readAltimeters();
  ICM::readICM();
  GPS::readGPS();

  // HAL::radio.beginPacket();
  // HAL::radio.print(Radio::downlink_packet);
  // HAL::radio.endPacket();
  // DEBUGLN("transmit");

  DEBUGLN();
  // delay(100);
}
