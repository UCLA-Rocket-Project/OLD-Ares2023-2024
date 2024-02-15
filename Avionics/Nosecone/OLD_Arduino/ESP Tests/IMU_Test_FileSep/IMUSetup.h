#ifndef IMUSETUP_H
#define IMUSETUP_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Globals.h"

#define IMU_CS 33

double IMU_temp;
double accel_x;
double accel_y;
double accel_z;
double gyro_x;
double gyro_y;
double gyro_z;

void setupIMU(Adafruit_ICM20948 &imu) {
  
  for (int i=0; i<30; i++) {
    if(imu.begin_SPI(IMU_CS, HSCK_PIN, HMISO_PIN, HMOSI_PIN)) {
      Serial.println("found");
      break;
    }
      
    Serial.println("Failed to find imu20948 chip");
  }

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
}

void readIMU(Adafruit_ICM20948 &imu) {
  
  /* Get a new normalized sensor event */
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
  Serial.println("read");
}

void printIMU(double t, double ax, double ay, double az, double gx, double gy, double gz) {

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Temp:");
  Serial.print(t);
  Serial.print("*C, ");


  Serial.print("\t\tAccel X: ");
  Serial.print(ax);
  Serial.print(", Y: ");
  Serial.print(ay);
  Serial.print(", Z: ");
  Serial.print(az);
  Serial.print(" m/s^2, ");

  Serial.print("\t\tGyro X: ");
  Serial.print(gx);
  Serial.print(", Y: ");
  Serial.print(gy);
  Serial.print(", Z: ");
  Serial.print(gz);
  Serial.println(" radians/s");
}


#endif
