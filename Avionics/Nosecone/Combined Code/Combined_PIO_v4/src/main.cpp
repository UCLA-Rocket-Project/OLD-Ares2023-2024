// #define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "GPS.h"
#include "BMP.h"
#include "ICM.h"
#include "XTSD.h"
#include "Radio.h"
#include <Arduino.h>



void radioGPS(void* param) {
  while(1) {
    if (millis() - GPS::lastGPSread > 1000) {
      GPS::readGPS();
    }

    if (millis() - Radio::lastTransmissionTime > 100) {

      Radio::downlink_packet = micros();
      Radio::downlink_packet += String(BMP::temp1, 2) + "," + String(BMP::pressure1, 2) + "," + String(BMP::altitude1, 2) + ",";
      // Radio::downlink_packet += String(BMP::temp2, 2) + "," + String(BMP::pressure2, 2) + "," + String(BMP::altitude2, 2) + ",";
      // Radio::downlink_packet += String(ICM::accel_x, 2) + "," + String(ICM::accel_y, 2) + "," + String(ICM::accel_z, 2) + ",";
      // Radio::downlink_packet += String(ICM::gyro_x, 2) + "," + String(ICM::gyro_y, 2) + "," + String(ICM::gyro_z, 2) + ",";
      // Radio::downlink_packet += String(GPS::latitude, 2) + "," + String(GPS::longitude, 2);
  
      int oldTime = millis();
      Radio::radio.beginPacket();
      Radio::radio.print(Radio::downlink_packet);
      Radio::radio.endPacket();
      Radio::lastTransmissionTime = millis() - oldTime;  
      DEBUG("transmit with transmission time: ");
      DEBUGLN(Radio::lastTransmissionTime);
      
    }
    // Serial.println("r");
  }
}

void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif
  
  HAL::initSensorHAL();

  GPS::setupGPS();
  BMP::setupBMP();
  ICM::setupIMU();
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

  BMP::readAltimeters();
  ICM::readIMU();

  int oldLog = millis();

  XTSD::logStr = (String)millis();
  XTSD::logStr += "," + String(XTSD::logTime) + "," + String(BMP::temp1, 2) + "," + String(BMP::pressure1, 2) + "," + String(BMP::altitude1, 2) + ",";
  // XTSD::logStr += String(BMP::temp2, 2) + "," + String(BMP::pressure2, 2) + "," + String(BMP::altitude2, 2) + ",";
  // XTSD::logStr += String(ICM::accel_x, 2) + "," + String(ICM::accel_y, 2) + "," + String(ICM::accel_z, 2) + ",";
  // XTSD::logStr += String(ICM::gyro_x, 2) + "," + String(ICM::gyro_y, 2) + "," + String(ICM::gyro_z, 2) + ",";
  // XTSD::logStr += String(GPS::latitude, 2) + "," + String(GPS::longitude, 2);
  
  XTSD::logSD(XTSD::logStr);
  XTSD::logTime = millis() - oldLog;
  // XTSD::i++;

  DEBUGLN();
  // delay(100);
}
