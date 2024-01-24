////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Pre-setup ---------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Adafruit_ICM20948 imu;

#define IMU_CS 37
#define IMU_SCK 13
#define IMU_MISO 12
#define IMU_MOSI 11

// Whether to log specific things
bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP ---------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);

  Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");


  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println();

  delay(100);

  //  Serial.print(temp.temperature);
  //
  //  Serial.print(",");
  //
  //  Serial.print(accel.acceleration.x);
  //  Serial.print(","); Serial.print(accel.acceleration.y);
  //  Serial.print(","); Serial.print(accel.acceleration.z);
  //
  //  Serial.print(",");
  //  Serial.print(gyro.gyro.x);
  //  Serial.print(","); Serial.print(gyro.gyro.y);
  //  Serial.print(","); Serial.print(gyro.gyro.z);
  //
  //  Serial.print(",");
  //  Serial.print(mag.magnetic.x);
  //  Serial.print(","); Serial.print(mag.magnetic.y);
  //  Serial.print(","); Serial.print(mag.magnetic.z);

  //  Serial.println();
  //
  //  delayMicroseconds(measurement_delay_us);
}
