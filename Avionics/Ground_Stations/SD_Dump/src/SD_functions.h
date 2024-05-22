#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H 

#include <SPI.h>
#include <SD.h>

String root = "/";



void filetree(File dir, int numTabs) {
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
      filetree(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void ls(String path) {

File dataFile = SD.open(path);
  while (true) {

    

    File entry =  dataFile.openNextFile();
    if (!entry) {
      // no more files
      Serial.println();
      break;
    }

    

    // if (entry.isDirectory()) {
    //   Serial.print("(");
    //   Serial.print(entry.name());
    //   Serial.print(")");
    // } else {
    //   Serial.print(entry.name());
    //   Serial.println(entry.size(), DEC);
    // }
    Serial.print(entry.name());
    entry.close();

    Serial.print("  ");
  }
}

void rm(String path) {

    if (SD.exists(path)) {
        SD.remove(path);
    }
    else {
        Serial.println("no file to delete");
    }
}

void touch(String path) {

    if (!SD.exists(path)) {
        SD.open(path).close();
        
    }
    else {
        Serial.println("no file to open");
    }
}

void cd(String root, String path) {

    if (path == "~") {
        root = "/";
    }
    if (!SD.exists(root + "/" + path)) {
        root = root + path;
        
    }
    else {
        Serial.println("no folder available");
    }
}

void pwd(String path) {

    Serial.println(path);
}

void nano(String path) {
  
  File dataFile = SD.open(path);
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
}



#endif