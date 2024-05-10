#ifndef MS_H
#define MS_H

#include "HAL.h"
#include "Libraries/MS5607/MS5607.h"

namespace MS {
    
    // RP_MS5607 ms;
    // MS5611_SPI MS5611(HAL::ALTIMETER2_CS, HAL::HMOSI_PIN, HAL::HMISO_PIN, HAL::HSCK_PIN);

    MS5607 ms5607;
    
    uint32_t start, stop;

    #define SEALEVELPRESSURE_HPA 1016.30 // Sea level pressure of LA 

    float temp, pressure, altitude;

    const float R = 287.052; // specific gas constant R*/M0
    const float g = 9.80665; // standard gravity 
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = 273.15 + 15; // temperature at 0 altitude
    

    void setupMS() {

        ms5607.begin();
        
    }


    
    int readAltimeter() {
        
        if(ms5607.readDigitalValue()){
            temp = ms5607.getTemperature();
            pressure = ms5607.getPressure();
            altitude = ms5607.getAltitude();
        }else{
            DEBUGLN("Error in reading digital value in sensor!");
            return 0;
        }     

        DEBUG("Altimeter 2: ");
        DEBUG(temp);
        DEBUG(" *C, ");
        DEBUG(pressure);
        DEBUG(" kPa, ");
        DEBUG(altitude);
        DEBUGLN(" m");
        
        return 1;
    }

    

}


#endif