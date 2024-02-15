#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "BMP.h"
#include "GPS.h"
#include "IMU.h"
#include "Radio.h"
#include <Arduino.h>


void radioGPS(void* param) {
  while(1) {
    if (millis() - GPS::lastGPSread > 1000) {
      GPS::readGPS();
    }

    if (millis() - Radio::lastTransmissionTime > 100) {
      int oldTime = millis();
      HAL::radio.beginPacket();
      HAL::radio.print(Radio::downlink_packet);
      HAL::radio.endPacket();
      Radio::lastTransmissionTime = millis() - oldTime;  
      DEBUGLN("transmit");
      
    }
    Serial.println("radioGPS");
  }
}



void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif

  HAL::initHAL();
  Radio::setupRadio();
  BMP::setupBMP();
  IMU::setupIMU();

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

  IMU::readIMU();
  BMP::readAltimeters();

  DEBUGLN();
  Serial.println("loop");
  delay(100);
}
