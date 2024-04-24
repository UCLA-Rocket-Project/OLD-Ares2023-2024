#include <SPI.h>
#include <LoRa.h>

const uint8_t SCK_PIN_A = 14;
const uint8_t MISO_PIN_A = 36;
const uint8_t MOSI_PIN_A = 13;
SPIClass* SPI_A;

const uint8_t SCK_PIN_B = 18;
const uint8_t MISO_PIN_B = 19;
const uint8_t MOSI_PIN_B = 23;
SPIClass* SPI_B;


const uint8_t CS_PIN_A = 15;
const uint8_t CS_PIN_B = 5;

const uint8_t RST_A = 16;
const uint8_t RST_B = 17;

const uint8_t D0_A = 32;
const uint8_t D1_A = 33;

const uint8_t D0_B = 25;
const uint8_t D1_B = 26;

#define syncWord 0x45
#define transmissionFreqA 915E6
#define transmissionFreqB 917E6

LoRaClass radioA;
LoRaClass radioB;

unsigned int decode_int(uint8_t* e, int s)
{
  return (uint32_t)e[s] <<  0 | (uint32_t)e[s + 1] <<  8 | (uint32_t)e[s + 2] << 16 | (uint32_t)e[s + 3] << 24;
}


unsigned short decode_short(uint8_t* e, int s)
{
  return (uint16_t)e[s] <<  0 | (uint16_t)e[s + 1] <<  8;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  SPI_A = new SPIClass(HSPI);
  SPI_A->begin(SCK_PIN_A, MISO_PIN_A, MOSI_PIN_A);

  SPI_B = new SPIClass(VSPI);
  SPI_B->begin(SCK_PIN_B, MISO_PIN_B, MOSI_PIN_B);

  radioA.setPins(CS_PIN_A, RST_A, D0_A);
  radioA.setSPI(*SPI_A);
  radioB.setPins(CS_PIN_B, RST_B, D0_B);
  radioB.setSPI(*SPI_B);


  
  if (!radioA.begin(transmissionFreqA)) {
    Serial.println("Starting LoRa A failed!");
  }
  if (!radioB.begin(transmissionFreqB)) {
    Serial.println("Starting LoRa B failed!");
  }

  radioA.setSyncWord(syncWord);
  radioB.setSyncWord(syncWord);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  
  // Radio A 
  int packetSizeA = radioA.parsePacket();
  if (packetSizeA) {
    // received a packet
    Serial.print("A,");

    // read packet
    while (radioA.available()) {
      Serial.print(radioA.read(), HEX);
    }

    // print RSSI of packet
//    Serial.print("' with RSSI ");
    Serial.print(",");
    Serial.println(radioA.packetRssi());
  }
  
  // Radio B
  int packetSizeB = radioB.parsePacket();
  if (packetSizeB) {
    // received a packet
    Serial.print("B,");

    // read packet
    while (radioB.available()) {
      Serial.print((char)radioB.read());
    }

    // print RSSI of packet
//    Serial.print("' with RSSI ");
    Serial.print(",");
    Serial.println(radioB.packetRssi());
  }
}
