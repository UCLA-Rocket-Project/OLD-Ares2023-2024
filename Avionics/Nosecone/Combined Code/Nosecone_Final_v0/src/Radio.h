#include "HAL.h"
#include "Globals.h"
// #include <RadioLib.h>
#include <Lora.h>

namespace Radio {

    #define syncWord 0x45
    #define transmissionFreq 915E6

    LoRaClass radio;

    int lastTransmissionTime = 0;

    String downlink_packet = (String)"9980.00,23.71" + "," + (String)lastTransmissionTime + ",";


    void setupRadio() {
        //replace the LoRa.begin(---E-) argument with your location's frequency 
        //915E6 for North America

        //setup LoRa transceiver module
        radio.setPins(HAL::TRANSMITTER_CS, HAL::TRANSMITTER_RST, HAL::TRANSMITTER_D0);
        radio.setSPI(*HAL::radioSPI);

        for (int i=0; i<300; i++) {
            if(radio.begin(transmissionFreq))
                break;    
            DEBUGLN(".");
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
        // delay(100);
    }

    // #define syncWord 0x45
    // #define transmissionFreq 915E6

    // SX1276* radio;

    // String dataRate = "";
    // int16_t err;


    // int lastTransmissionTime = 0;
    // int oldTime = 0;

    // String downlink_packet = (String)"9980.00,23.71" + "," + (String)lastTransmissionTime + ",";


    // void setupRadio() {
        
    //     radio = new SX1276(new Module(HAL::TRANSMITTER_CS, HAL::TRANSMITTER_D1, HAL::TRANSMITTER_RST, HAL::TRANSMITTER_D0, *HAL::radioSPI));
    
    //     Serial.print(F("[SX1276] Initializing ... "));
    //     int state = radio->begin(915.0,125.0,7,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);

    //     #ifdef DEBUGMODE
    //     if (state == RADIOLIB_ERR_NONE) {
    //         Serial.println(F("success!"));
    //     } else {
    //         Serial.print(F("failed, code "));
    //         Serial.println(state);
    //     //    while (true);
    //     }
    //     #endif
        
    // }


    // void transmitPacket() {

    //     oldTime = millis();

    //     err = radio->transmit(downlink_packet);
    
    //     #ifdef DEBUGMODE
    //     if (err == RADIOLIB_ERR_NONE) {

    //         Serial.println(F("Transmission Success! Datarate:\t"));
    //         Serial.print(radio->getDataRate());
    //         Serial.println(F(" bps"));
            
    //     } else {
    //         // some error occurred
    //         Serial.print(F("failed, code "));
    //         Serial.println(err);
    //     }
    //     #endif

    // }

    
}