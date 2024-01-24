////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes -------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h> // Needed for I2C to GPS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS

SFE_UBLOX_GNSS myGNSS;
long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.

long latitude = 0;
long longitude = 0;
byte SIV = 0;
  
double oldTime = millis();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);

 
  Serial.println("Initializing GPS");

  
  Wire.begin(); // Idk why this is 2 and not 1
//  myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial
  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
//  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
//  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP ---------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  
    if (millis() - lastTime > 100)
    {
      lastTime = millis(); //Update the timer
      latitude = myGNSS.getLatitude();
      longitude = myGNSS.getLongitude();
      SIV = myGNSS.getSIV();

      
      Serial.print(F("Lat: "));
      Serial.print(latitude);
      Serial.print(F(" Long: "));
      Serial.print(longitude);
      Serial.print(F(" (degrees * 10^-7)"));
      Serial.print(F(" SIV: "));
      Serial.println(SIV);
  }
  


//  delay(1000);

}
