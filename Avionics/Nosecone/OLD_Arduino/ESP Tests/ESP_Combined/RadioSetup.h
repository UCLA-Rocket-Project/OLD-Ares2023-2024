#ifndef RADIOSETUP_H
#define RADIOSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <LoRa.h>

#define FREQ 915E6

#define RADIO_CS 5
#define RADIO_RST 27
#define RADIO_D0 2
#define RADIO_D1 4

int pkg = 0;
int state = 0;
int lastTransmissionTime = 0;
int oldTime = 0;

void setupRadio(long freq, int syncWord) {

  
  //setup LoRa transceiver module
  LoRa.setPins(RADIO_CS, RADIO_RST, RADIO_D0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //915E6 for North America
  while (!LoRa.begin(freq)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(syncWord);
  Serial.println("LoRa Initializing OK!");
}

#endif
