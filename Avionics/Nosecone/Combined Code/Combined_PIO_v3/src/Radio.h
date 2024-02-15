#include "HAL.h"

namespace Radio {

    #define syncWord 0x45
    #define transmissionFreq 915E6

    int lastTransmissionTime = 0;

    String downlink_packet = (String)"9980.00,23.71" + "," + (String)lastTransmissionTime + ",";


    void setupRadio() {
        //replace the LoRa.begin(---E-) argument with your location's frequency 
        //915E6 for North America
        while (!HAL::radio.begin(transmissionFreq)) {
            DEBUGLN(".");
            delay(500);
        }
        // Change sync word to match the receiver
        HAL::radio.setSyncWord(syncWord);
        DEBUGLN("LoRa Initializing OK!");
    }


    void transmitPacket() {
        int oldTime = millis();
        HAL::radio.beginPacket();
        HAL::radio.print(downlink_packet);
        HAL::radio.endPacket();
        lastTransmissionTime = millis() - oldTime;  
        DEBUGLN("transmit");
        // DEBUGLN();
        delay(100);
    }
}