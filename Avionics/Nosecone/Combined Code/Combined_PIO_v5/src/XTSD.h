#ifndef XTSD_H
#define XTSD_H

#include "HAL.h"
#include <SD.h>


namespace XTSD {
    
    File dataFile;
    const char* fileName = "/TestCode/test10.txt";

    String logStr = "";

    int logTime = 0;

    void setupSD() {
        if(!SD.begin(HAL::SD_CS, *HAL::sensorSPI)) {
            Serial.println("initialization failed!");
            return;
        } 
        // return true;
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