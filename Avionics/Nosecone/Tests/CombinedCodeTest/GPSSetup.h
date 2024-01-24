#ifndef GPSSETUP_H
#define GPSSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h> // Needed for I2C to GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include "Globals.h"

long lastGPS = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

long latitude = 0;
long longitude = 0;
byte SIV = 0;



int setupGPS(SFE_UBLOX_GNSS &gps, bool debug = false) {
  
    Wire2.begin(); // Idk why this is 2 and not 1
    
    if (debug)
      gps.enableDebugging();
      
    if (gps.begin(Wire2, 0x42) == false) //Connect to the u-blox module using Wire port
      return 0;
    else {
//      myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
//      myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
      return 1;
    }
  
  
}

void readGPS(SFE_UBLOX_GNSS &gps) {
  latitude = gps.getLatitude();
  longitude = gps.getLongitude();
  SIV = gps.getSIV();

  if (print_values) {
    Serial.print(F("Lat: "));
    Serial.print(latitude);
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));
    Serial.print(F(" SIV: "));
    Serial.println(SIV);
  }
  
}

#endif
