#ifndef RADIOSETUP_H
#define RADIOSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <RadioLib.h>

#define FREQ 915.0

#define RADIO_CSS 10
#define DIO0 4
#define RESET 9
#define DIO1 3

int pkg = 0;
int state = 0;
int lastTransmissionTime = 0;

long lastRadio = 0;

void setupRadio(SX1276 &radio, double freq) {
  state = radio.begin(freq,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);
  if (print_values) {
    switch(state) {
    case RADIOLIB_ERR_NONE:
        Serial.println(F("success!"));
      break;
    default:
      Serial.print(F("failed, code "));
      Serial.println(state);
      break;
    }
  }
}

void transmitRadio(SX1276 &radio, uint8_t* str, int str_size) {
  
  digitalWrite(LED_BUILTIN, LOW);
  
  pkg = radio.transmit(str, str_size);
  
  if(print_values) {
    if (pkg == RADIOLIB_ERR_NONE) {

      Serial.println(F("Success! Datarate:\t"));
      Serial.print(radio.getDataRate());
      Serial.println(F(" bps"));
      
    } else {
      // some error occurred
      Serial.print(F("failed, code "));
      Serial.println(pkg);
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void transmitRadio(SX1276 &radio, String str) {
  
  digitalWrite(LED_BUILTIN, LOW);
  
  pkg = radio.transmit(str);
  
  if(print_values) {
    if (pkg == RADIOLIB_ERR_NONE) {

      Serial.println(F("Success! Datarate:\t"));
      Serial.print(radio.getDataRate());
      Serial.println(F(" bps"));
      
    } else {
      // some error occurred
      Serial.print(F("failed, code "));
      Serial.println(pkg);
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

#endif
