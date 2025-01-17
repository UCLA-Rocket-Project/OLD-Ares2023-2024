#include <SPI.h>
#include <LoRa.h>

#define RADIO_CS 5
#define RADIO_RST 14
#define RADIO_D0 2
#define RADIO_D1 4

int counter = 0;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(RADIO_CS, RADIO_RST, RADIO_D0);
    
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("12345678901234");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

//  delay(100);
}
