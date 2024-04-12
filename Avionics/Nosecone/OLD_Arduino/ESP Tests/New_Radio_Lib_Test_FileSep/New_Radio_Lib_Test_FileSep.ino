#include "Globals.h"
#include "RadioSetup.h"

void setup() {

  Serial.begin(115200);
//  pinMode(LED_BUILTIN, OUTPUT);

  setupRadio(FREQ, 0xF3);

  xTaskCreatePinnedToCore (
    loop2,     // Function to implement the task
    "loop2",   // Name of the task
    10000,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );
  
}

void loop() {
  
}


void loop2(void* pvParameters) {
  while (1) {
    downlink_packet = (String)"9980.00,23.71,100.642,57.15,19.52,-0.51,-0.71,-0.04,0.00,0.00,0,0,0,914.97,684" + "," + (String)lastTransmissionTime + ",";

   
    int oldTime = millis();
    LoRa.beginPacket();
  //  LoRa.print("12345678901234");
    LoRa.print(downlink_packet);
    LoRa.endPacket();
    lastTransmissionTime = millis() - oldTime;  
    Serial.println("transmit");
  }
}
