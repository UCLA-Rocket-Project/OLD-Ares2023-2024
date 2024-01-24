#ifndef BMPSETUP_H
#define BMPSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11

#define BMP1_CS 26
#define BMP2_CS 38

#define SEALEVELPRESSURE_HPA (1013.25)

double temp1;
double pressure1;
double altitude_m1;
double temp2;
double pressure2;
double altitude_m2;

void setupBMP() {
  if (reading_BMP) {
    if (! bmp1.begin_SPI(BMP1_CS)) {  // hardware SPI mode  
      Serial.println("Could not find a valid BMP3 sensor, check wiring!");
      while (1);
    }
  
    if (! bmp2.begin_SPI(BMP2_CS)) {  // hardware SPI mode  
      Serial.println("Could not find a valid BMP3 sensor, check wiring!");
      while (1);
    }
  }
}


#endif
