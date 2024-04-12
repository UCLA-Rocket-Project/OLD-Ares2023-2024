#ifndef SDSETUP_H
#define SDSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SD.h>
#include <SPI.h>

#include "Globals.h"

#define SD_CS 13

int logSD(File &dataFile, const char fileName[], String str) {

  dataFile = SD.open(fileName, FILE_WRITE);
  
  if (dataFile) {
    
    if (print_values) {
      Serial.print("Writing to ");
      Serial.print(fileName);
      Serial.println("...");
    }
    
    dataFile.println(str);

    dataFile.close();
    
    if (print_values)
      Serial.println("done.");
    return 1;
  } 
  else
    return 0;
}


#endif
