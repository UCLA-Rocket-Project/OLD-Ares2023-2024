#include<SPI.h> //include to get the meanings of SS, MISO, MOSI, SCK, etc.
int i = 0;
byte txData[] = {0x12, 0x34, 0x56};
byte rxData[3];
volatile bool flag = false;
bool flag1 = false;

void setup()
{
  Serial.begin(9600);
  pinMode(SS, INPUT_PULLUP); // ensure SS stays high for now
  pinMode(MISO, OUTPUT); //must do
  pinMode(MOSI, INPUT); //always do
  pinMode(SCK, INPUT); //always do
  bitSet(SPCR, SPE); //SPI Port is enabled
  bitClear(SPCR,MSTR); //Arduino is Slave
  SPDR = 0x98;
  SPI.attachInterrupt(); //interrupt logic is enabled
}
void loop()
{
  if (flag == true)
  {
    rxData[i] = SPDR;
    Serial.print(rxData[i], HEX);
    SPDR = txData[i]; //places 0x12, then 0x34, then 0x56, then 0xAB
    i++;
    if (i == 3) //3-byte data are sent
    {
      i = 0; //array pointer is reset
      Serial.println();
    }
    flag = false;
  }
}

ISR(SPI_STC_vect)
{
  flag = true;
}
