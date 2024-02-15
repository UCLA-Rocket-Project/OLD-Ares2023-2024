#define DEBUG_MODE

/*
  Listfiles

  This example shows how print out the files in a
  directory on a SD card

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 2 Feb 2014
  by Scott Fitzgerald

  This example code is in the public domain.

*/
#include <SPI.h>
#include <SD.h>
#include "Globals.h"
#include "HAL.h"

File root;

void printDirectory(File, int);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  HAL::initSensorHAL();
  
  if (!SD.begin(HAL::SD_CS, *HAL::sensorSPI)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  // printDirectory(root, 0);

  // const char* dir_to_add = "/sup";
  // if (!SD.exists(dir_to_add)) {
  //   Serial.println("no directory");
  //   SD.mkdir(dir_to_add);
  // }

  printDirectory(root, 0);

  Serial.println("done!");
}

void loop() {


  

  if (Serial.available() > 0) {
    // read the incoming byte:
    String fileName;
    fileName = Serial.readString();

    File dataFile = SD.open(fileName);

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening file");
    }
    Serial.println("Done!");
    root = SD.open("/");
    printDirectory(root, 0);
  }

  

}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}


