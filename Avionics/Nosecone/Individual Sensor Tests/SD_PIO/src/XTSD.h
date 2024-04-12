#ifndef XTSD_H
#define XTSD_H

#include "HAL.h"



namespace XTSD {
    
    File dataFile;
    const char* fileName = "/TestCode/yo.txt";

    bool setupSD() {

        if(!SD.begin(HAL::SD_CS, *HAL::sensorSPI)) {
            Serial.println("initialization failed!");
            return false;
        } 
        
        return true;
    }

    void logSD(String str) {

        dataFile = SD.open(fileName, FILE_APPEND);
        
        if (dataFile) {
            
            DEBUG("Writing to ");
            DEBUG(fileName);
            DEBUGLN("...");
            
            dataFile.println(str);

            dataFile.close();
            
            DEBUGLN("done.");
        }
        else {
            DEBUG("Error writing to ");
            DEBUGLN(fileName);
        }
    }   

}


#endif