////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes -------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// include the library for Radio Module
#include <RadioLib.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Include libraries for the sensors
#include <SD.h>
#include <Wire.h>
#include <SPI.h>

// Include revelant libraries for BMP388 Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

// Include revelant libraries for ICM20948 Sensor
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>

// Include revelant libraries for GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Pre-setup ---------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SX1276 has the following connections:
// CS pin:   10
// DIO0 pin:  4
// RESET pin: 9
// DIO1 pin:  3
SX1276 radio = new Module(10, 4, 9, 3);


// Define pins
#define SENSOR_SCK 13
#define SENSOR_MISO 12
#define SENSOR_MOSI 11


// For SPI mode, we need a Chip Select (CS) pin
#define ICM_CS 37
#define BMP_CS 38

Adafruit_ICM20948 icm;
Adafruit_BMP3XX bmp;

// GPS module

SFE_UBLOX_GNSS myGNSS;
long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

const int cs_SD = BUILTIN_SDCARD;
static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
String str = "";
int package = 0;
double temp;
double pressure;
double altitude_m;
double accel_x;
double accel_y;
double accel_z;
double gyro_x;
double gyro_y;
double gyro_z;
long latitude = 0;
long longitude = 0;
byte SIV = 0;

bool reading_GPS = true;
bool reading_IMU = true;
bool reading_BMP = true;
bool do_transmit = true;
bool do_log = true;

  
double oldTime = millis();
//int encode(double x, uint8_t* e, int s) {
//  uint8_t* d = (uint8_t*) &x;
//  e[s] = (uint8_t)(d >>  0);
//  e[s + 1] = (uint8_t)(d >>  8);
//  e[s + 2] = (uint8_t)(d >> 16);
//  e[s + 3] = (uint8_t)(d >> 24);
//  e[s + 4] = (uint8_t)(d >> 32);
//  e[s + 5] = (uint8_t)(d >> 40);
//  e[s + 6] = (uint8_t)(d >> 48);
//  e[s + 7] = (uint8_t)(d >> 56);
//  return s + 8;
//}

File dataFile;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- FUNCTIONS ----------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int encode(unsigned short x, uint8_t* e, int s) {
  e[s] = (uint8_t)(x >>  0);
  e[s + 1] = (uint8_t)(x >>  8);
  return s + 2;
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  Serial.println();

  
//  Serial.print(F("  Date/Time: "));
//  if (gps.date.isValid())
//  {
//    Serial.print(gps.date.month());
//    Serial.print(F("/"));
//    Serial.print(gps.date.day());
//    Serial.print(F("/"));
//    Serial.print(gps.date.year());
//  }
//  else
//  {
//    Serial.print(F("INVALID"));
//  }
  
//  Serial.print(F(" "));
//  if (gps.time.isValid())
//  {
//    if (gps.time.hour() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.hour());
//    Serial.print(F(":"));
//    if (gps.time.minute() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.minute());
//    if (gps.time.second() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.second());
//    Serial.print(F("."));
//    if (gps.time.centisecond() < 10) Serial.print(F("0"));
//    Serial.print(gps.time.centisecond());
//  }
//  else
//  {
//    Serial.print(F("INVALID"));
//  }
//  Serial.println();
}

/*
 * Functions to write:
 * 1. Read BMP with chip select pin
 * 2. Read ICM data
 * 3. 
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Constants/Variables -----------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SEALEVELPRESSURE_HPA (1013.25) // get readings and define zero whereever you are
int PACKET_SIZE = 2;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);


  Serial.print("Initializing SD card...");
//   see if the card is present and can be initialized:
  if (!SD.begin(cs_SD)) {
    Serial.println("Card failed, or not present");
//    while (1) {
//      // No SD card, so don't do anything more - stay stuck here
//    }
  }
  Serial.println("card initialized.");
  

  
  // initialize SX1276 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(915.0,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
//    while (true);
  }

// WHAT IS THIS?!?!?!?!?
  // RX enable:   4
  // TX enable:   5
//  radio.setRfSwitchPins(5, 4);


  Serial.println("Adafruit BMP388 / BMP390 test");
//  if (! bmp.begin_SPI(BMP_CS, SENSOR_SCK, SENSOR_MISO, SENSOR_MOSI)) {  // hardware SPI mode
  if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  
  // Initialize ICM20649 Sensor
  Serial.println("Adafruit ICM20649 test!");
  // Try to initialize!
//  if (!icm.begin_SPI(ICM_CS, SENSOR_SCK, SENSOR_MISO, SENSOR_MOSI)) {
  if (!icm.begin_SPI(ICM_CS)) {
    Serial.println("Failed to find ICM20649 chip");
    while (1) {
      delay(10);
    }
  }

  
  Serial.println("ICM20948 Found!");
  // Setting precision of accelerometer
  // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");
  switch (icm.getAccelRange()) {
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
  // Setting precision of gyroscope
  // icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
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
  
  //  icm.setAccelRateDivisor(4095);
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);
  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);
  //  icm.setGyroRateDivisor(255);
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);
  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);
  Serial.println();
  Serial.println("Initializing GPS");
  ss.begin(GPSBaud);
  
  Wire2.begin();
//  myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial
  if (myGNSS.begin(Wire2, 0x42) == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR







  
  
//  delay(3000);


  
  
  
//  reading_GPS = false;
//  reading_IMU = false;
//  reading_BMP = false;
  do_transmit = false;
  //do_log = false;
}

//unsigned double test = 10.0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP ---------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  uint8_t e[PACKET_SIZE];

  double currentTime = millis();
  str = String(currentTime);
  
  if (reading_BMP) {
    // loop through BMP388
    if (! bmp.performReading()) {
      Serial.println("Failed to perform reading :(");
      return;
    }
    temp = bmp.temperature;
    pressure = bmp.pressure;
    altitude_m = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    
  //  Serial.print("Temperature: ");
  //  Serial.print(temp);
  //  Serial.println(" *C");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" kPa");
    Serial.print("Altitude: ");
    Serial.print(altitude_m);
    Serial.println(" m");
    str += "," +String(temp,2) + "," + String(pressure,3) + "," + String(altitude_m,2) + ",";
  }
  
  
  if (reading_IMU) {
    // loop through ICM20948
    //  /* Get a new normalized sensor event */
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    icm.getEvent(&accel, &gyro, &temp);
    // read values from ICM20948
    accel_x = accel.acceleration.x;
    accel_y = accel.acceleration.y;
    accel_z = accel.acceleration.z;
    gyro_x = gyro.gyro.x;
    gyro_y = gyro.gyro.y;
    gyro_z = gyro.gyro.z;
    //  The BMP388 already prints the temperature
      Serial.print("Temperature ");
      Serial.print(temp.temperature);
      Serial.println(" deg C");
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("Accel X: ");
    Serial.print(accel_x);
    Serial.print(" ");
    Serial.print("Y: ");
    Serial.print(accel_y);
    Serial.print(" ");
    Serial.print("Z: ");
    Serial.print(accel_z);
    Serial.println(" m/s^2");
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("Gyro X: ");
    Serial.print(gyro_x);
    Serial.print(" ");
    Serial.print("Y: ");
    Serial.print(gyro_y);
    Serial.print(" ");
    Serial.print("Z: ");
    Serial.print(gyro_z);
    Serial.println(" radians/s");
    // send the GPS data
  }
  

  
  if (reading_GPS) {
    if (millis() - lastTime > 1000)
      {
      lastTime = millis(); //Update the timer
      latitude = myGNSS.getLatitude();
      Serial.print(F("Lat: "));
      Serial.print(latitude);
      longitude = myGNSS.getLongitude();
      Serial.print(F(" Long: "));
      Serial.print(longitude);
      Serial.print(F(" (degrees * 10^-7)"));
  
      SIV = myGNSS.getSIV();
      Serial.print(F(" SIV: "));
      Serial.println(SIV);
  
 
    while (ss.available() > 0)
      if (gps.encode(ss.read()))
        displayInfo();
    }
  }

  str += String(accel_x,2) + "," + String(accel_y,2) + "," + String(accel_z,2) + ",";
  str += String(gyro_x,2) + "," + String(gyro_y,2) + "," + String(gyro_z,2)+ ",";
  str += String(latitude,2) + "," + String(longitude,2) + "," + String(SIV,2);

    
  if (do_transmit) {
    

    Serial.print(F("[SX1276] Transmitting packet ... "));
//    // you can transmit C-string or Arduino string up to
//    // 256 characters long
//    // NOTE: transmit() is a blocking method!
//    //       See example SX127x_Transmit_Interrupt for details
//    //       on non-blocking transmission method.
//  
//    unsigned short uPressure;
//    uPressure = (unsigned short) pressure;
//    encode(uPressure, e, 0);
//    Serial.print(F(" pressure test: "));
//    Serial.println(uPressure);
//    encode(temp, e, 8);
    digitalWrite(LED_BUILTIN, LOW);
    package = radio.transmit(e, PACKET_SIZE);
    int pkg = radio.transmit(str);
    if (pkg == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      Serial.println(F(" success!"));
      // print measured data rate
      Serial.print(F("[SX1278] Datarate:\t"));
      Serial.print(radio.getDataRate());
      Serial.println(F(" bps"));
    } else if (pkg == RADIOLIB_ERR_PACKET_TOO_LONG) {
      // the supplied packet was longer than 256 bytes
      Serial.println(F("too long!"));
  } else if (pkg == RADIOLIB_ERR_TX_TIMEOUT) {
      // timeout occurred while transmitting packet
      Serial.println(F("timeout!"));
    } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(pkg);
    }
    
  }



// if the file is available, write to it:

  
  if (do_log) {
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(str);
      dataFile.close();
      Serial.println("logged to SD card");
    } else {
      // if the file isn't open, pop up an error:
      Serial.println("error opening datalog.txt");
    }
  }


  double timeDiff = currentTime - oldTime;
  oldTime = currentTime;
  Serial.print("Time diff: ");
  Serial.println(timeDiff);
  Serial.println();
//  Serial.println(str);
  
  // wait for a second before transmitting again
//  delay(1000);
}
