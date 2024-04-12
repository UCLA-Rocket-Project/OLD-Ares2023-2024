#ifndef SDSETUP_H
#define SDSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SD.h>
#include <SPI.h>

#include "BMPSetup.h"
#include "IMUSetup.h"
#include "GPSSetup.h"

#include "Globals.h"

#define SD_CS BUILTIN_SDCARD

String logStr = "";

String create_file() {
  int n = 0;
  String file_name = String(n) + ".txt";
//  const char* file_name = s;
  while (SD.exists(file_name.c_str()))
  {
    n++;
    file_name = String(n) + ".txt";
  }
  return file_name;
}

int logSD(File &dataFile, String fileName, String str) {

  dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  
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
