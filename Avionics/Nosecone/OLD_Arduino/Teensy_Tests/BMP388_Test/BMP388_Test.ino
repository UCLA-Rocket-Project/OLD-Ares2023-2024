////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <SPI.h>
#include "BMPSetup.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Pre-setup ---------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Adafruit_BMP3XX bmp1;
Adafruit_BMP3XX bmp2;

// Whether to log specific things
bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP ---------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  if (reading_BMP) {
    if (! bmp1.performReading()) {
      Serial.println("Failed to read Altimeter 1 :(");
      return;
    }
    else {
      temp1 = bmp1.temperature;
      pressure1 = bmp1.pressure / 1000.0;
      altitude_m1 = bmp1.readAltitude(SEALEVELPRESSURE_HPA);
    }
    if (! bmp2.performReading()) {
      Serial.println("Failed to read Altimeter 2 :(");
      return;
    }
    else {
      temp2 = bmp2.temperature;
      pressure2 = bmp2.pressure / 1000.0;
      altitude_m2 = bmp2.readAltitude(SEALEVELPRESSURE_HPA);
    }
    
    
    if (print_values) {
      Serial.print("Temperatures = ");
      Serial.print(temp1);
      Serial.print(" *C, ");
      Serial.print(temp2);
      Serial.println(" *C");

      Serial.print("Pressures = ");
      Serial.print(pressure1);
      Serial.print(" kPa, ");
      Serial.print(pressure2);
      Serial.println(" kPa");
      
      Serial.print("Approx. Altitudes = ");
      Serial.print(altitude_m1);
      Serial.print(" m, ");
      Serial.print(altitude_m2);
      Serial.println(" m");
      
    } // if Printing values
   
  } // if Reading BMP
  


  Serial.println();
  delay(1000);
}
