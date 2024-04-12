// #define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "BMP.h"
#include "GPS.h"
#include "ICM.h"
#include "Radio.h"
#include <Arduino.h>


void radioGPS(void* param) {
  while(1) {
    if (millis() - GPS::lastGPSread > 1000) {
      GPS::readGPS();
    }

    if (millis() - Radio::lastTransmissionTime > 100) {

      Radio:: downlink_packet = micros();
      Radio::downlink_packet += String(BMP::temp1, 2) + "," + String(BMP::pressure1, 2) + "," + String(BMP::altitude1, 2) + ",";
      // Radio::downlink_packet += String(BMP::temp2, 2) + "," + String(BMP::pressure2, 2) + "," + String(BMP::altitude2, 2) + ",";
      // Radio::downlink_packet += String(ICM::accel_x, 2) + "," + String(ICM::accel_y, 2) + "," + String(ICM::accel_z, 2) + ",";
      // Radio::downlink_packet += String(ICM::gyro_x, 2) + "," + String(ICM::gyro_y, 2) + "," + String(ICM::gyro_z, 2) + ",";
      // Radio::downlink_packet += String(GPS::latitude, 2) + "," + String(GPS::longitude, 2);
  
      int oldTime = millis();
      HAL::radio.beginPacket();
      HAL::radio.print(Radio::downlink_packet);
      HAL::radio.endPacket();
      Radio::lastTransmissionTime = millis() - oldTime;  
      DEBUG("transmit with transmission time: ");
      DEBUGLN(Radio::lastTransmissionTime);
      
    }
    Serial.println("radioGPS");
  }
}

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
  ICM::readICM();

  // Radio::downlink_packet = millis();
  // Radio::downlink_packet += String(BMP::temp1, 2) + "," + String(BMP::pressure1, 2) + "," + String(BMP::altitude1, 2) + ",";
  // Radio::downlink_packet += String(BMP::temp2, 2) + "," + String(BMP::pressure2, 2) + "," + String(BMP::altitude2, 2) + ",";
  // Radio::downlink_packet += String(ICM::accel_x, 2) + "," + String(ICM::accel_y, 2) + "," + String(ICM::accel_z, 2) + ",";
  // Radio::downlink_packet += String(ICM::gyro_x, 2) + "," + String(ICM::gyro_y, 2) + "," + String(ICM::gyro_z, 2) + ",";
  // Radio::downlink_packet += String(GPS::latitude, 2) + "," + String(GPS::longitude, 2);
  

  DEBUGLN();
  // delay(100);
}
