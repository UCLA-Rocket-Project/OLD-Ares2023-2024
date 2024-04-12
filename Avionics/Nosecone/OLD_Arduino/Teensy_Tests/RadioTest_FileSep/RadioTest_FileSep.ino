#include "Globals.h"
#include "RadioSetup.h"

SX1276 radio = new Module(5, 4, 9, 3);

void setup() {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  setupRadio(radio, FREQ);
}

void loop() {


  
//  downlink_packet = (String)oldTime + "," + (String)(radio.getDataRate()) + "," + (String)lastTransmissionTime + ",";
  downlink_packet = "9980.00,23.71,100.642,57.15,19.52,-0.51,-0.71,-0.04,0.00,0.00,0,0,0,914.97,684";
  int packet_size = downlink_packet.length();
  
  encodeMessage(downlink_packet, encoded_packet, packet_size);
  Serial.println(downlink_packet);
  int oldTime = millis();
  transmitRadio(radio, encoded_packet, MAXLENGTH);
  lastTransmissionTime = millis() - oldTime; 

  
}
