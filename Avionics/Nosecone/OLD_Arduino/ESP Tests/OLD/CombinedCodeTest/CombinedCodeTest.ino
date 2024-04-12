////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "BMPSetup.h"
#include "IMUSetup.h"
#include "GPSSetup.h"
#include "SDSetup.h"
#include "RadioSetup.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Object Declaration ------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Adafruit_BMP3XX bmp1;
Adafruit_BMP3XX bmp2;

Adafruit_ICM20948 imu;

SFE_UBLOX_GNSS gps;

SX1276 radio = new Module(RADIO_CSS, DIO0, RESET, DIO1);

File dataFile;
String fileName;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  /* --------------------------------- For Testing --------------------------------- */
//  reading_GPS = 0;
//  reading_IMU = 0;
//  reading_BMP = 0;
//  do_transmit = 0;
//  do_log = 0;
  print_values = 0;

  /* --------------------------------- Start Serial --------------------------------- */
  Serial.begin(115200);
  while (!Serial);
  

  if (do_transmit) {
    pinMode(LED_BUILTIN, OUTPUT);
    setupRadio(radio, FREQ);
  }
  

  if (reading_BMP) {
    setupAltimeters(bmp1, bmp2);
    Serial.println("Successfully initialized BMP Modules");
  }

  if (reading_IMU) {
    if (!imu.begin_SPI(IMU_CS)) {
      Serial.println("Failed to find imu20948 chip");
      while (1);
    }
    Serial.println("Successfully initialized IMU Module");
  }

  if(do_log) {
    if(!SD.begin(BUILTIN_SDCARD)) 
      Serial.println("SD Initialization failed!");
    else
      Serial.println("Successfully initialized SD Module");

    fileName = create_file();
  }
  
  if(reading_GPS) {
    if(!setupGPS(gps)) {
      Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring."));
    }
    else 
      Serial.println("Successfully initialized GPS Module");
  }
  Serial.println("done");
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

  /* --------------------------------- GPS --------------------------------- */
  
  if (reading_GPS && (millis() - lastGPS > 3000)) {

    readGPS(gps);
    if (print_values) {
//        printGPS();
    }
    lastGPS = millis();
  }
  

  /* --------------------------------- Prepare String for Transmit --------------------------------- */

  if (do_log || do_transmit) {
    downlink_packet = String(currentTime) + ",";
    
//    downlink_packet += String(temp1,2) + "," + String(pressure1,2) + "," + String(altitude_m1,2) + ",";
//    downlink_packet += String(temp2,2) + "," + String(pressure2,2) + "," + String(altitude_m2,2) + ",";
//    downlink_packet += String(accel_x,2) + "," + String(accel_y,2) + "," + String(accel_z,2) + ",";
//    downlink_packet += String(gyro_x,2) + "," + String(gyro_y,2) + "," + String(gyro_z,2)+ ",";
//    downlink_packet += String(latitude,2) + "," + String(longitude,2) + "," + String(SIV,2);

    downlink_packet +=  String(pressure1,2) + "," + String( ((altitude_m1 + altitude_m2)/2.0) ,2) + ",";
    downlink_packet += String(latitude,2) + "," + String(longitude,2) + "," + String(SIV,2);
  }
  

  /* --------------------------------- If Transmitting --------------------------------- */

  if (do_transmit && (millis() - lastRadio > 1000)) {

    int packet_size = downlink_packet.length();
  
//    encodeMessage(downlink_packet, encoded_packet, packet_size);
  
    int oldTime = millis();
//    transmitRadio(radio, encoded_packet, MAXLENGTH);
    transmitRadio(radio, downlink_packet);
    lastTransmissionTime = millis() - oldTime; 

    lastRadio = millis();
  }

  /* --------------------------------- Logger --------------------------------- */

  if (do_log) {
    if (!logSD(dataFile, fileName, downlink_packet)){
      if (print_values) {
        Serial.print("Error writing to ");
        Serial.println(fileName);
      }
    }
    else {
      if (print_values) {
        Serial.print("Logged to ");
        Serial.println(fileName);
      }
    }
    
  } // if Logging
  
  
//  Serial.println(downlink_packet);
//  Serial.println();
//  delay(1000);
}
