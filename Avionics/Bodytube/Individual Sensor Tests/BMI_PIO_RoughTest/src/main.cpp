#include <Arduino.h>
#include <Wire.h>

#define INC_ADDRESS 0x68
#define ACC_CONF  0x20  //Page 91
#define GYR_CONF  0x21  //Page 93
#define CMD       0x7E  //Page 65

uint16_t x, y, z;
uint16_t gyr_x, gyr_y, gyr_z;

uint16_t  temperature = 0;
float     temperatureInDegree = 0.f;

void setup(void) {  
  Serial.begin(115200); 
  //Accelerometer
  Wire.begin();  
  //Wire.setClock(400000);      // I2C Fast Mode (400kHz)  
  softReset();  
  /*
   * Acc_Conf P.91
   * mode:        0x7000  -> High
   * average:     0x0000  -> No
   * filtering:   0x0080  -> ODR/4
   * range:       0x0000  -> 2G
   * ODR:         0x000B  -> 800Hz
   * Total:       0x708B
   */
  writeRegister16(ACC_CONF,0x708B);//Setting accelerometer  
  /*
   * Gyr_Conf P.93
   * mode:        0x7000  -> High
   * average:     0x0000  -> No
   * filtering:   0x0080  -> ODR/4
   * range:       0x0000  -> 125kdps
   * ODR:         0x000B  -> 800Hz
   * Total:       0x708B
   */
  writeRegister16(GYR_CONF,0x708B);//Setting gyroscope    
}

void softReset(){  
  writeRegister16(CMD, 0xDEAF);
  delay(50);    
}

void loop() {

  if(readRegister16(0x02) == 0x00) {
    //Read ChipID
    Serial.print("ChipID:");
    Serial.print(readRegister16(0x00));    
    readAllAccel();             // read all accelerometer/gyroscope/temperature data     
    Serial.print(" \tx:");
    Serial.print(x);
    Serial.print(" \ty:");
    Serial.print(y);
    Serial.print(" \tz:");
    Serial.print(z);
    Serial.print(" \tgyr_x:");
    Serial.print(gyr_x);
    Serial.print(" \tgyr_y:");
    Serial.print(gyr_y);
    Serial.print(" \tgyr_z:");
    Serial.print(gyr_z);
    Serial.print(" \ttemp:");
    Serial.println(temperatureInDegree);    
  }
}

//Write data in 16 bits
void writeRegister16(uint16_t reg, uint16_t value) {
  Wire.beginTransmission(INC_ADDRESS);
  Wire.write(reg);
  //Low 
  Wire.write((uint16_t)value & 0xff);
  //High
  Wire.write((uint16_t)value >> 8);
  Wire.endTransmission();
}

//Read data in 16 bits
uint16_t readRegister16(uint8_t reg) {
  Wire.beginTransmission(INC_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  int n = Wire.requestFrom(INC_ADDRESS, 4);  
  uint16_t data[20];
  int i =0;
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }  
  return (data[3]   | data[2] << 8);
}

//Read all axis
void readAllAccel() {
  Wire.beginTransmission(INC_ADDRESS);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(INC_ADDRESS, 20);
  uint16_t data[20];
  int i =0;
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }

  //Offset = 2 because the 2 first bytes are dummy (useless)
  int offset = 2;  
  x =             (data[offset + 0]   | (uint16_t)data[offset + 1] << 8);  //0x03
  y =             (data[offset + 2]   | (uint16_t)data[offset + 3] << 8);  //0x04
  z =             (data[offset + 4]   | (uint16_t)data[offset + 5] << 8);  //0x05
  gyr_x =         (data[offset + 6]   | (uint16_t)data[offset + 7] << 8);  //0x06
  gyr_y =         (data[offset + 8]   | (uint16_t)data[offset + 9] << 8);  //0x07
  gyr_z =         (data[offset + 10]  | (uint16_t)data[offset + 11] << 8); //0x08
  temperature =   (data[offset + 12]  | (uint16_t)data[offset + 13] << 8); //0x09
  temperatureInDegree = (temperature/512.f) + 23.0f;  
}