//#include "Globals.h"
#include <RadioLib.h>

// CS pin:   10 - 5 - 10
// DIO0 pin:  4 - 2 - 2
// RESET pin: 9 - 14 - 9
// DIO1 pin:  3 - 3 - 3
SX1276 radio = new Module(5, 2, 14, 3);

String dataRate = "";
int16_t err;
String packet = "";

bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;



void setup() {
  Serial.begin(115200);
//  pinMode(LED_BUILTIN, OUTPUT);

  
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(915.0,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);

  if (print_values) {
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
  //    while (true);
    }
  }

  xTaskCreatePinnedToCore (
    loop2,     // Function to implement the task
    "loop2",   // Name of the task
    1000,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );
}

void loop() {
  

  packet = (String)millis() + "ms, ";

  

  dataRate = (String)(radio.getDataRate());

  
  
  
  packet += dataRate + ", ";

//  digitalWrite(LED_BUILTIN, LOW);






  
  err = radio.transmit("12345678901234");

  
//  delay(500);
//  err = radio.finishTransmit();
  if(print_values) {
    if (err == RADIOLIB_ERR_NONE) {

      Serial.println(F("Transmission Success! Datarate:\t"));
      Serial.print(radio.getDataRate());
      Serial.println(F(" bps"));
      
    } else {
      // some error occurred
      Serial.print(F("failed, code "));
      Serial.println(err);
    }
  }

}

void loop2(void* pvParameters) {
  while (1) {
    Serial.print ("Hello");
    delay (500); // wait for half a second
    Serial.print (" Datarate of the radio ");
    Serial.println (dataRate);
    delay (500); // wait for half a second
  }
}
