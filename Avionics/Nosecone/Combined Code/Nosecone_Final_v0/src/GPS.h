#include "HAL.h"
#include <TinyGPSPlus.h>

namespace GPS {

    TinyGPSPlus gps;

    double latitude = 0;
    double longitude = 0;
    double GPS_alt = 0;

    int lastGPSread = 0;

    void setupGPS() {
        HAL::gps_serial.begin(9600);
    }

    void readGPS() {
    
        while (HAL::gps_serial.available() > 0)
            if (gps.encode(HAL::gps_serial.read()))

        if (millis() > 5000 && gps.charsProcessed() < 10)
        {
            DEBUGLN(F("No GPS detected: check wiring."));
            while(true);
        }

        
        // lastTime = millis(); //Update the timer
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        GPS_alt = gps.altitude.meters();

        if (gps.time.isValid())
        {
            DEBUG(F("Time: "));
            if (gps.time.hour() < 10) DEBUG(F("0"));
            DEBUG(gps.time.hour());
            DEBUG(F(":"));
            if (gps.time.minute() < 10) DEBUG(F("0"));
            DEBUG(gps.time.minute());
            DEBUG(F(":"));
            if (gps.time.second() < 10) DEBUG(F("0"));
            DEBUG(gps.time.second());
            DEBUG(F("."));
            if (gps.time.centisecond() < 10) DEBUG(F("0"));
            DEBUG(gps.time.centisecond());
        }
        DEBUG(F(" Lat: "));
        #ifdef DEBUG_MODE
            Serial.print(latitude, 6);
        #endif
        DEBUG(F(" Long: "));
        #ifdef DEBUG_MODE
            Serial.print(longitude, 6);
        #endif
        DEBUG(F(" Alt: "));
        DEBUG(GPS_alt);
        DEBUG(F("Chars Processed"));
        DEBUG(gps.charsProcessed());
        DEBUGLN();

    }
    
}

    
