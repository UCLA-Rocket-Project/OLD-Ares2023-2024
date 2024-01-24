#include <SPI.h>
byte sendData[] = {0xAB, 0xCD, 0xEF, 0x04};
byte recData[4];
void setup ()
{
 Serial.begin(9600);
 SPI.begin(); //default speed: 4 MHz
 digitalWrite(SS, LOW); //Slave is selected
}
void loop()
{
 for (int i = 0; i < 3; i++)
 {
 recData[i] = SPI.transfer(sendData[i]);
 Serial.print("recData["); Serial.print(i); Serial.print("] = ");
 Serial.print(recData[i], HEX);
 Serial.print(" from Slave's sData[");
 Serial.print(i-1);
 Serial.println("] ");
 }
 long y = (long)recData[1] << 16 | (long)recData[2] << 8 | (long)recData[0];
 Serial.println(y, HEX);
 Serial.print("Data Received from Slave: ");
 Serial.println("=========================");
 delay(3000);
}
