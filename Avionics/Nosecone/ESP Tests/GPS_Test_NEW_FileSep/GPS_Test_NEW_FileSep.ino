////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "GPSSetup.h"
#include "Globals.h"

// The TinyGPSPlus object
TinyGPSPlus gps;

//HardwareSerial gps_serial(0);

void setup()
{
  Serial.begin(9600);

  if (reading_GPS) {
    setupGPS(Serial);
  }
}

void loop()
{
  if (reading_GPS && (millis() - lastTime > 3000)) {
    readGPS(gps, Serial);
    
  }
  
}
