#ifndef GPSSETUP_H
#define GPSSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include "Globals.h"

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
long latitude = 0;
long longitude = 0;

static const uint32_t GPSBaud = 9600;

void setupGPS(HardwareSerial &gps_port) {

  if (gps_port != Serial) {
    gps_port.begin(GPSBaud);
    
  }
}

void readGPS(TinyGPSPlus &gps, HardwareSerial &gps_port) {
  
  while (gps_port.available() > 0)
    if (gps.encode(gps_port.read()))

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  
  lastTime = millis(); //Update the timer
  latitude = gps.location.lat();
  longitude = gps.location.lng();

  if (1)
  {
    Serial.print(F("Lat: "));
    Serial.print(latitude);
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.println();
//  Serial.print(F(" (degrees * 10^-7)"));
  }
  
}






void displayInfo(TinyGPSPlus &gps)
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

#endif
