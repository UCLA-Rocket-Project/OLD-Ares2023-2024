#ifndef GPSSETUP_H
#define GPSSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h> // Needed for I2C to GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include "Globals.h"

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

long latitude = 0;
long longitude = 0;
byte SIV = 0;

#define I2C_SDA 16
#define I2C_SCL 17
TwoWire I2CGPS = TwoWire(0);

int setupGPS(SFE_UBLOX_GNSS &gps, bool debug = false) {
  
    Wire.begin();
    
    if (debug)
      gps.enableDebugging();
      
    if (!gps.begin(I2CGPS, 0x42)) //Connect to the u-blox module using Wire port
      return 0;
    else {
      gps.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
      gps.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
      return 1;
    }
  
  
}

void readGPS(SFE_UBLOX_GNSS &gps) {
  lastTime = millis(); //Update the timer
  latitude = gps.getLatitude();
  longitude = gps.getLongitude();
  SIV = gps.getSIV();

  
  Serial.print(F("Lat: "));
  Serial.print(latitude);
  Serial.print(F(" Long: "));
  Serial.print(longitude);
  Serial.print(F(" (degrees * 10^-7)"));
  Serial.print(F(" SIV: "));
  Serial.println(SIV);
}

#endif
