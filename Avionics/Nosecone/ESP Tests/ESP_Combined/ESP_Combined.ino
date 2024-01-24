////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "BMPSetup.h"
#include "IMUSetup.h"
#include "GPSSetup.h"
#include "RadioSetup.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Object Declaration ------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Adafruit_BMP3XX bmp1;
Adafruit_BMP3XX bmp2;

Adafruit_ICM20948 imu;

TinyGPSPlus gps;
HardwareSerial gps_serial(2);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {
  Serial.begin(9600);

  //Very important
  disableCore0WDT();
  disableCore1WDT();

  setupRadio(FREQ, 0xF3);

  /* --------------------------------- Altimeter --------------------------------- */
  if (reading_BMP) {
    setupAltimeters(bmp1, bmp2);
  }

  /* --------------------------------- IMU --------------------------------- */
  if (reading_IMU) {
    setupIMU(imu);

  }

  /* --------------------------------- GPS --------------------------------- */
  if (reading_GPS) {
    setupGPS(gps_serial);
  }

  // Parallelizes
  //  xTaskCreatePinnedToCore (
  //    loop1,     // Function to implement the task
  //    "LoggerLoop",   // Name of the task
  //    2000,      // Stack size in words
  //    NULL,      // Task input parameter
  //    1,         // Priority of the task
  //    NULL,      // Task handle.
  //    1          // Core where the task should run
  //  );

  xTaskCreatePinnedToCore (
    radioGPS,     // Function to implement the task
    "radioGPS",   // Name of the task
    2000,      // Stack size in words
    NULL,      // Task input parameter
    2,         // Priority of the task
    NULL,      // Task handle.
    0           // Core where the task should run
  );
  Serial.println("uhh");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP ---------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {

  int currentTime = millis();
  /* --------------------------------- Altimeter --------------------------------- */
  if (reading_BMP) {
    if (!readAltimeter(bmp1, temp1, pressure1, altitude_m1)) {
      Serial.println("Failed to read Altimeter 1 :(");
    }
    if (!readAltimeter(bmp2, temp2, pressure2, altitude_m2)) {
      Serial.println("Failed to read Altimeter 2 :(");
    }

    if (print_values) {
      printAltimeter(1, temp1, pressure1, altitude_m1);
      printAltimeter(2, temp2, pressure2, altitude_m2);
    }

  }

  /* --------------------------------- IMU --------------------------------- */
  if (reading_IMU) {
    readIMU(imu);

    if (print_values) {
      printIMU(IMU_temp, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z);
    }
  }



  /* --------------------------------- Prepare String for Transmit --------------------------------- */

  if (do_log || do_transmit) {
    downlink_packet = String(currentTime) + ",";
    //
    downlink_packet += String(temp1, 2) + "," + String(pressure1, 2) + "," + String(altitude_m1, 2) + ",";
    downlink_packet += String(temp2, 2) + "," + String(pressure2, 2) + "," + String(altitude_m2, 2) + ",";
    downlink_packet += String(accel_x, 2) + "," + String(accel_y, 2) + "," + String(accel_z, 2) + ",";
    downlink_packet += String(gyro_x, 2) + "," + String(gyro_y, 2) + "," + String(gyro_z, 2) + ",";
    downlink_packet += String(latitude, 2) + "," + String(longitude, 2);
    //
    ////    downlink_packet +=  String(pressure1,2) + "," + String( ((altitude_m1 + altitude_m2)/2.0) ,2) + ",";
    ////    downlink_packet += String(latitude,2) + "," + String(longitude,2) + "," + String(SIV,2);
  }

  Serial.println(downlink_packet);
  delay(10);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP2 --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void radioGPS(void * pvParameters) {
  while (1) {
    Serial.println("other");
    delay(100); // Need a small delay in the code to prevent traffic i guess
    /* --------------------------------- GPS --------------------------------- */
    if (millis() - lastTime > 3000) {
      readGPS(gps, gps_serial);
      Serial.println("GPS");
    }

    /* --------------------------------- Transmit --------------------------------- */
    if (do_transmit) {
      oldTime = millis();
      LoRa.beginPacket();
      LoRa.print(downlink_packet);
      LoRa.endPacket();
      lastTransmissionTime = millis() - oldTime;
      Serial.println("Transmit");
    }

    vTaskDelay(10);
  }
}
