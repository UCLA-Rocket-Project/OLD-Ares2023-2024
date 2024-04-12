#include "HAL.h"
#include <Lora.h>

namespace Radio {

    #define syncWord 0x45
    #define transmissionFreq 917E6

    LoRaClass radio;

    int lastTransmissionTime = 0;

    String downlink_packet = (String)"9980.00,23.71" + "," + (String)lastTransmissionTime + ",";


    void setupRadio() {
        
        //replace the LoRa.begin(---E-) argument with your location's frequency 
        //915E6 for North America

        //setup LoRa transceiver module
        radio.setPins(HAL::TRANSMITTER_CS, HAL::TRANSMITTER_RST, HAL::TRANSMITTER_D0);

        while (!radio.begin(transmissionFreq)) {
            DEBUGLN(".");
            delay(500);
        }
        // Change sync word to match the receiver
        radio.setSyncWord(syncWord);
        DEBUGLN("LoRa Initializing OK!");
    }


    void transmitPacket() {
        int oldTime = millis();
        radio.beginPacket();
        radio.print(downlink_packet);
        radio.endPacket();
        lastTransmissionTime = millis() - oldTime;  
        DEBUGLN("transmit");
        // DEBUGLN();
        delay(100);
    }
}