#ifndef HAL_H
#define HAL_H

#include "Globals.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>

#include <LoRa.h>

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Variables                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




namespace HAL { 

    // VSPI
    const uint8_t VSCK_PIN = 18;
    const uint8_t VMISO_PIN = 19;
    const uint8_t VMOSI_PIN = 23;
    SPIClass* radioSPI;

    // HSPI
    const uint8_t HSCK_PIN = 14;
    const uint8_t HMISO_PIN = 36;
    const uint8_t HMOSI_PIN = 13;
    SPIClass* sensorSPI;

    // I2C
    const uint8_t INA_SDA_PIN = 21;
    const uint8_t INA_SCL_PIN = 22;

    // Serial 
    const uint8_t GPS_RX_PIN = 16;
    const uint8_t GPS_TX_PIN = 17;
    const uint8_t GPS_I2C_PORT = 2; //This is not a pin

    // Flight data
    Adafruit_BMP3XX bmp1;
    Adafruit_BMP3XX bmp2;
    Adafruit_ICM20948 imu;
    TinyGPSPlus gps;

    /* ---------- Pinouts ---------- */
    // Transmitter
    const uint8_t TRANSMITTER_RST = 27;
    const uint8_t TRANSMITTER_CS = 5;
    const uint8_t TRANSMITTER_D0 = 2;
    const uint8_t TRANSMITTER_D1 = 4;

    // Altimeters
    const uint8_t ALTIMETER1_CS = 26;
    const uint8_t ALTIMETER2_CS = 25;

    // IMU
    const uint8_t IMU_CS = 33;

    // SD/Flash
    const uint8_t SD_CS = 32;

    void initSensorHAL() {
        
        sensorSPI = new SPIClass(HSPI);
        sensorSPI->begin(HSCK_PIN, HMISO_PIN, HMOSI_PIN);
        

    }   
}


#endif