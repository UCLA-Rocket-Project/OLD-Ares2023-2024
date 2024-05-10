#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>
const uint8_t SCK_PIN_A = 14;
const uint8_t MISO_PIN_A = 36;
const uint8_t MOSI_PIN_A = 13;
SPIClass* SPI_A;

const uint8_t SCK_PIN_B = 18;
const uint8_t MISO_PIN_B = 19;
const uint8_t MOSI_PIN_B = 23;
SPIClass* SPI_B;


const uint32_t CS_PIN_A = 15;
const uint8_t CS_PIN_B = 5;

const uint32_t RST_A = 16;
const uint8_t RST_B = 17;

const uint32_t D0_A = 32;
const uint8_t D1_A = 33;

const uint32_t D0_B = 25;
const uint8_t D1_B = 26;

#define syncWord 0x45
#define transmissionFreqA 915E6
#define transmissionFreqB 917E6

SX1276* radio_A;
SX1276* radio_B;

int state_A;
int state_B;

String str_A;
String str_B;


// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio_A= RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  SPI_A = new SPIClass(HSPI);
  SPI_A->begin(SCK_PIN_A, MISO_PIN_A, MOSI_PIN_A);
  
  SPI_B = new SPIClass(VSPI);
  SPI_B->begin(SCK_PIN_B, MISO_PIN_B, MOSI_PIN_B);


  
  // initialize SX1278 with default settings
  Serial.print(F("Radio A Initializing ... "));
  radio_A = new SX1276(new Module(CS_PIN_A, D0_A, RST_A, D1_A, *SPI_A));
  int state = radio_A->begin(915.0,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }


  
  // Serial.print(F("Radio B Initializing ... "));

  // radio_B = new SX1276(new Module(CS_PIN_B, D0_B, RST_B, D1_B, *SPI_A));
  // state = radio_B->begin(917.0,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);
  // if (state == RADIOLIB_ERR_NONE) {
  //   Serial.println(F("success!"));
  // } else {
  //   Serial.print(F("failed, code "));
  //   Serial.println(state);
  //   while (true);
  // }

}

void loop() {
  Serial.print(F("[SX1278] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  str_A = "";
  // str_B = "";

  state_A = radio_A->receive(str_A);
  // state_B = radio_B->receive(str_B);

  if (state_A == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[SX1278] Data:\t\t\t"));
    Serial.println(str_A);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[SX1278] RSSI:\t\t\t"));
    Serial.print(radio_A->getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("[SX1278] SNR:\t\t\t"));
    Serial.print(radio_A->getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    // of the last received packet
    Serial.print(F("[SX1278] Frequency error:\t"));
    Serial.print(radio_A->getFrequencyError());
    Serial.println(F(" Hz"));

  } else if (state_A == RADIOLIB_ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else if (state_A == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state_A);

  }

  // if (state_B == RADIOLIB_ERR_NONE) {
  //   // packet was successfully received
  //   Serial.println(F("success!"));

  //   // print the data of the packet
  //   Serial.print(F("[SX1278] Data:\t\t\t"));
  //   Serial.println(str_B);

  //   // print the RSSI (Received Signal Strength Indicator)
  //   // of the last received packet
  //   Serial.print(F("[SX1278] RSSI:\t\t\t"));
  //   Serial.print(radio_B->getRSSI());
  //   Serial.println(F(" dBm"));

  //   // print the SNR (Signal-to-Noise Ratio)
  //   // of the last received packet
  //   Serial.print(F("[SX1278] SNR:\t\t\t"));
  //   Serial.print(radio_B->getSNR());
  //   Serial.println(F(" dB"));

  //   // print frequency error
  //   // of the last received packet
  //   Serial.print(F("[SX1278] Frequency error:\t"));
  //   Serial.print(radio_B->getFrequencyError());
  //   Serial.println(F(" Hz"));

  // } else if (state_B == RADIOLIB_ERR_RX_TIMEOUT) {
  //   // timeout occurred while waiting for a packet
  //   Serial.println(F("timeout!"));

  // } else if (state_B == RADIOLIB_ERR_CRC_MISMATCH) {
  //   // packet was received, but is malformed
  //   Serial.println(F("CRC error!"));

  // } else {
  //   // some other error occurred
  //   Serial.print(F("failed, code "));
  //   Serial.println(state_B);

  // }
}