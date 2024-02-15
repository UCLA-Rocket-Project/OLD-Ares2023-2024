#ifndef GPS_H
#define GPS_H

#include "HAL.h"

namespace GPS {

    long latitude = 0;
    long longitude = 0;
    double GPS_alt = 0;

    int lastGPSread = 0;

    void setupGPS() {
        HAL::gps_serial.begin(9600);
    }
    void readGPS() {
    
        while (HAL::gps_serial.available() > 0)
            if (HAL::gps.encode(HAL::gps_serial.read()))

        if (millis() > 5000 && HAL::gps.charsProcessed() < 10)
        {
            DEBUGLN(F("No GPS detected: check wiring."));
            while(true);
        }

        
        // lastTime = millis(); //Update the timer
        latitude = HAL::gps.location.lat();
        longitude = HAL::gps.location.lng();
        GPS_alt = HAL::gps.altitude.meters();

        if (HAL::gps.time.isValid())
        {
            DEBUG(F("Time: "));
            if (HAL::gps.time.hour() < 10) DEBUG(F("0"));
            DEBUG(HAL::gps.time.hour());
            DEBUG(F(":"));
            if (HAL::gps.time.minute() < 10) DEBUG(F("0"));
            DEBUG(HAL::gps.time.minute());
            DEBUG(F(":"));
            if (HAL::gps.time.second() < 10) DEBUG(F("0"));
            DEBUG(HAL::gps.time.second());
            DEBUG(F("."));
            if (HAL::gps.time.centisecond() < 10) DEBUG(F("0"));
            DEBUG(HAL::gps.time.centisecond());
        }
        DEBUG(F(" Lat: "));
        DEBUG(latitude);
        DEBUG(F(" Long: "));
        DEBUG(longitude);
        DEBUG(F(" Alt: "));
        DEBUG(GPS_alt);
        DEBUG(F("Chars Processed"));
        DEBUG(HAL::gps.charsProcessed());
        DEBUGLN();

    }
    
}

#endif
    
