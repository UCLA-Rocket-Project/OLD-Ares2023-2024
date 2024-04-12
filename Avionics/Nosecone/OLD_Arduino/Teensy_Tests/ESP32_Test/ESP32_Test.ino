////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <SPI.h>
#include <LoRa.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Pre-setup ---------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Adafruit_ICM20948 imu;

#define IMU_CS 33

#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
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


  if (reading_IMU) {
    if (!imu.begin_SPI(IMU_CS)) {
      Serial.println("Failed to find imu20948 chip");
      while (1) {
        delay(10);
      }
    }
    Serial.println("imu20948 Found!");
    // imu.setAccelRange(imu20948_ACCEL_RANGE_16_G);
    Serial.print("Accelerometer range set to: ");
    switch (imu.getAccelRange()) {
    case ICM20948_ACCEL_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case ICM20948_ACCEL_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case ICM20948_ACCEL_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case ICM20948_ACCEL_RANGE_16_G:
      Serial.println("+-16G");
      break;
    }
    Serial.println("OK");
  
    // imu.setGyroRange(imu20948_GYRO_RANGE_2000_DPS);
    Serial.print("Gyro range set to: ");
    switch (imu.getGyroRange()) {
    case ICM20948_GYRO_RANGE_250_DPS:
      Serial.println("250 degrees/s");
      break;
    case ICM20948_GYRO_RANGE_500_DPS:
      Serial.println("500 degrees/s");
      break;
    case ICM20948_GYRO_RANGE_1000_DPS:
      Serial.println("1000 degrees/s");
      break;
    case ICM20948_GYRO_RANGE_2000_DPS:
      Serial.println("2000 degrees/s");
      break;
    }
  
    //  imu.setAccelRateDivisor(4095);
    uint16_t accel_divisor = imu.getAccelRateDivisor();
    float accel_rate = 1125 / (1.0 + accel_divisor);
  
    Serial.print("Accelerometer data rate divisor set to: ");
    Serial.println(accel_divisor);
    Serial.print("Accelerometer data rate (Hz) is approximately: ");
    Serial.println(accel_rate);
  
    //  imu.setGyroRateDivisor(255);
    uint8_t gyro_divisor = imu.getGyroRateDivisor();
    float gyro_rate = 1100 / (1.0 + gyro_divisor);
  
    Serial.print("Gyro data rate divisor set to: ");
    Serial.println(gyro_divisor);
    Serial.print("Gyro data rate (Hz) is approximately: ");
    Serial.println(gyro_rate);
  
    Serial.println();
  }
  
}

void loop() {
  Serial.print("Sending packet: ");
  
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(accel_x);
  LoRa.print(",");
  LoRa.print(accel_y);
  LoRa.print(",");
  LoRa.print(accel_z);
  LoRa.endPacket();

  counter++;

//  delay(100);
}



void loop2(void * parameter) {

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp);

  IMU_temp = temp.temperature;
  accel_x = accel.acceleration.x;
  accel_y = accel.acceleration.y;
  accel_z = accel.acceleration.z;
  gyro_x = gyro.gyro.x;
  gyro_y = gyro.gyro.y;
  gyro_z = gyro.gyro.z;

  
}
