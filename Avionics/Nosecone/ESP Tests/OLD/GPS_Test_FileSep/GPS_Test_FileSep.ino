////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes -------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GPSSetup.h"
#include "Globals.h"

SFE_UBLOX_GNSS gps;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- SETUP --------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  
  Serial.println("Initializing GPS");
  I2CGPS.begin(I2C_SDA, I2C_SCL, 100000);
  if(reading_GPS) {
    while(!setupGPS(gps)) {
      Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring."));
    }
  }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- LOOP ---------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  
  if (millis() - lastTime > 1000) {
      readGPS(gps);
  }
  


//  delay(100);

}
