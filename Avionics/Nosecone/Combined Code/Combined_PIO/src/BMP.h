#ifndef BMP_H
#define BMP_H

#include "HAL.h"



namespace BMP {
    
    #define SEALEVELPRESSURE_HPA 1011.10 // Sea level pressure of LA 

    double temp1, pressure1, altitude1;
    double temp2, pressure2, altitude2;

    void setupBMP() {
        for (int i=0; i<3000; i++) {
            if(HAL::bmp1.begin_SPI(HAL::ALTIMETER1_CS, HAL::sensorSPI))
                break;    
            DEBUGLN("Failed to find BMP 1");
        }

        for (int i=0; i<3000; i++) {
            if(HAL::bmp2.begin_SPI(HAL::ALTIMETER2_CS, HAL::sensorSPI))
                break;
            DEBUGLN("Failed to find BMP 2");
        }
    }


    
    int readAltimeters() {
  
        if (! HAL::bmp1.performReading()) {
            return 0;
        }
        else {
            temp1 = HAL::bmp1.temperature;
            pressure1 = HAL::bmp1.pressure / 1000.0;
            altitude1 = HAL::bmp1.readAltitude(SEALEVELPRESSURE_HPA);
        }

        if (! HAL::bmp2.performReading()) {
            return 0;
        }
        else {
            temp2 = HAL::bmp2.temperature;
            pressure2 = HAL::bmp2.pressure / 1000.0;
            altitude2 = HAL::bmp2.readAltitude(SEALEVELPRESSURE_HPA);
        }

        DEBUG("Altimeter 1: ");
        DEBUG(temp1);
        DEBUG(" *C, ");
        DEBUG(pressure1);
        DEBUG(" kPa, ");
        DEBUG(altitude1);
        DEBUGLN(" m");
        DEBUG("Altimeter 2: ");
        DEBUG(temp2);
        DEBUG(" *C, ");
        DEBUG(pressure2);
        DEBUG(" kPa, ");
        DEBUG(altitude2);
        DEBUGLN(" m");

        return 1;

    }

    void printAltimeter() {
        
    }

}


#endif