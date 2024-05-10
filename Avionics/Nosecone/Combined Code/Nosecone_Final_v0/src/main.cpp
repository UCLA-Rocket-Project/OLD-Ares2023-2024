#define DEBUG_MODE

#include "Globals.h"
#include "HAL.h"
#include "GPS.h"
#include "BMP.h"
#include "ICM.h"
#include "XTSD.h"
#include "MS.h"
#include "INA.h"
#include "Radio.h"
#include <Arduino.h>



void radioGPS(void* param) {
  while(1) {
    if (millis() - GPS::lastGPSread > 1000) {
      GPS::readGPS();
    }

    if (millis() - Radio::lastTransmissionTime > 100) {

      // Radio::downlink_packet = millis();
      // Radio::downlink_packet += "," + String(BMP::temp1, 2) + "," + String(BMP::pressure1, 2) + "," + String(BMP::altitude1, 2) + ",";
      // // Radio::downlink_packet += String(BMP::temp2, 2) + "," + String(BMP::pressure2, 2) + "," + String(BMP::altitude2, 2) + ",";
      // Radio::downlink_packet += String(ICM::accel_x, 2) + "," + String(ICM::accel_y, 2) + "," + String(ICM::accel_z, 2) + ",";
      // // Radio::downlink_packet += String(ICM::gyro_x, 2) + "," + String(ICM::gyro_y, 2) + "," + String(ICM::gyro_z, 2) + ",";
      // Radio::downlink_packet += String(GPS::latitude, 6) + "," + String(GPS::longitude, 6) + "," + String(GPS::GPS_alt, 1) + ",";
      // Radio::downlink_packet += String(INA::bus_voltage, 2) + "," + XTSD::logSuccess;
  
      // Radio::lastTransmissionTime = millis();
      // Radio::radio.beginPacket();
      // Radio::radio.print(Radio::downlink_packet);
      // Radio::radio.endPacket();
      // Radio::lastTransmissionTime = millis() - Radio::lastTransmissionTime;  
      // DEBUG("transmit with transmission time: ");
      // DEBUGLN(Radio::lastTransmissionTime);
      
    }
    // Serial.println("r");
  }
}

void setup() {
  Serial.begin(9600);

  #ifdef DEBUG_MODE
    while(!Serial);
  #endif


  INA::setupINA();

  HAL::initCSPins();
  HAL::initSensorHAL();
  
  MS::setupMS();
  // delay(500);
  GPS::setupGPS();
  // delay(500);
  BMP::setupBMP();
  // delay(500);
  ICM::setupIMU();
  // delay(500);
  
  XTSD::setupSD();
  
  HAL::initRadioHAL();
  

  // Radio::setupRadio();
  
    
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
  MS::readAltimeter();
  
  ICM::readIMU();
  
  INA::readINA();
  
  int oldTime = micros();
  XTSD::logStr = (String)micros();
  XTSD::logStr += "," + String(XTSD::logTime) + "," + String(BMP::temp1, 2) + "," + String(BMP::pressure1, 2) + "," + String(BMP::altitude1, 2) + ",";
  // XTSD::logStr += String(BMP::temp2, 2) + "," + String(BMP::pressure2, 2) + "," + String(BMP::altitude2, 2) + ",";
  XTSD::logStr += String(ICM::accel_x, 2) + "," + String(ICM::accel_y, 2) + "," + String(ICM::accel_z, 2) + ",";
  XTSD::logStr += String(ICM::gyro_x, 2) + "," + String(ICM::gyro_y, 2) + "," + String(ICM::gyro_z, 2) + ",";
  XTSD::logStr += String(GPS::latitude, 6) + "," + String(GPS::longitude, 6), + "," + String(GPS::GPS_alt, 2) + ",";
  XTSD::logStr += String(INA::bus_voltage);

  XTSD::logSD(XTSD::logStr);
  XTSD::logTime = micros() - oldTime;
  // XTSD::i++;

  DEBUGLN();
  delay(100);
}
