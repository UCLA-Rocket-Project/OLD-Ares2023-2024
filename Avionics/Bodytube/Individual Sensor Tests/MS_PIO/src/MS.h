#ifndef MS_H
#define MS_H

#include "HAL.h"
#include "RP_MS5607.h"
#include "MS5611_SPI.h"

namespace MS {
    
    // RP_MS5607 ms;
    // MS5611_SPI MS5611(HAL::ALTIMETER2_CS, HAL::HMOSI_PIN, HAL::HMISO_PIN, HAL::HSCK_PIN);
    MS5611_SPI ms5607(HAL::ALTIMETER_CS, HAL::sensorSPI);
    
    uint32_t start, stop;

    #define SEALEVELPRESSURE_HPA 1018.90 // Sea level pressure of LA 

    float temp, pressure, altitude;

    const float R = 287.052; // specific gas constant R*/M0
    const float g = 9.80665; // standard gravity 
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = 273.15 + 15; // temperature at 0 altitude
    
    void setupMS() {

        // ms.init(HAL::ALTIMETER2_CS, HAL::sensorSPI);

        // uint32_t start = micros();

        // while (micros() - start < 3000)     //  increased as first ROM values were missed.
        // {
        //     yield();
        //     delayMicroseconds(10);
        // }

        // ms.readDeviceID();
        // ms.readCalibrationValues();

        // DEBUG("MS5607 ID: ");
        // DEBUGLN(ms.getDeviceID(), HEX);


        ms5607.setSPIport(HAL::sensorSPI);

        if (ms5607.begin() == true)
        {
            DEBUG("MS5607 found: ");
            // DEBUGLN(ms5607.getDeviceID(), HEX);
        }

        if (ms5607.reset(1) == true)
        {
            DEBUG("MS5611 conversion set to MS5607");
        }
        
    }


    
    int readAltimeter() {
  
        ms5607.read();
        temp = ms5607.getTemperature();
        pressure = ms5607.getPressure();
        altitude = t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure / SEALEVELPRESSURE_HPA)));


        // DEBUG("Altimeter 2 (MS5607): ");
        DEBUG(temp);
        DEBUG(",");
        // DEBUG(" *C, ");
        DEBUG(pressure);
        DEBUG(",");
        // DEBUG(" Pa, ");
        DEBUGLN(altitude);
        // DEBUGLN(" m");
        return 1;
    }

    

}


#endif