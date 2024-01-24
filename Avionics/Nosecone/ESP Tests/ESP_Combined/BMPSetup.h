#ifndef BMPSETUP_H
#define BMPSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include "Globals.h"

#define BMP1_CS 26
#define BMP2_CS 25

#define SEALEVELPRESSURE_HPA (1013.25)

double temp1;
double pressure1;
double altitude_m1;
double temp2;
double pressure2;
double altitude_m2;

void setupAltimeters(Adafruit_BMP3XX &bmp1, Adafruit_BMP3XX &bmp2) {

  for (;;) {
//  for (int i=0; i<30; i++) {
    if(bmp1.begin_SPI(BMP1_CS, HSCK_PIN, HMISO_PIN, HMOSI_PIN))
      break;
    Serial.println("Failed to find BMP 1");
  }

  for (int i=0; i<30; i++) {
    if(bmp2.begin_SPI(BMP2_CS, HSCK_PIN, HMISO_PIN, HMOSI_PIN))
      break;
    Serial.println("Failed to find BMP 2");
  }
 
  
}



int readAltimeter(Adafruit_BMP3XX &sensor, double &t, double &p, double &a) {
  
  if (! sensor.performReading()) {
    return 0;
  }
  else {
    t = sensor.temperature;
    p = sensor.pressure / 1000.0;
    a = sensor.readAltitude(SEALEVELPRESSURE_HPA);
    return 1;
  }
}

void printAltimeter(int num, double &t, double &p, double &a) {
  Serial.print("Altimeter ");
  Serial.print(num);
  Serial.print(": ");
  Serial.print(t);
  Serial.print(" *C, ");
  Serial.print(p);
  Serial.print(" kPa, ");
  Serial.print(a);
  Serial.println(" m");
}

#endif
