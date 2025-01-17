#include "Arduino.h"
#include "RP_H3LIS.h"

#include "Globals.h"
#include "HAL.h"



H3LIS xl;

void setup() 
{

  Serial.begin(9600);

  HAL::initSensorHAL();

  xl.begin(comm_mode::USE_SPI, HAL::Shock_CS, HAL::sensorSPI); // Selects the bus to be used and sets
                          //  the power up bit on the accelerometer.
                          //  Also zeroes out all accelerometer
                          //  registers that are user writable.

  // // This next section configures an interrupt. It will cause pin
  // //  INT1 on the accelerometer to go high when the absolute value
  // //  of the reading on the Z-axis exceeds a certain level for a
  // //  certain number of samples.
  // xl.intSrcConfig(H3LIS::INT_SRC, 1); // Select the source of the
  //                         //  signal which appears on pin INT1. In
  //                         //  this case, we want the corresponding
  //                         //  interrupt's status to appear. 
  // xl.setIntDuration(50, 1); // Number of samples a value must meet
  //                         //  the interrupt condition before an
  //                         //  interrupt signal is issued. At the
  //                         //  default rate of 50Hz, this is one sec.
  // xl.setIntThreshold(2, 1); // Threshold for an interrupt. This is
  //                         //  not actual counts, but rather, actual
  //                         //  counts divided by 16.
  // xl.enableInterrupt(H3LIS::Z_AXIS, H3LIS::TRIG_ON_HIGH, 1, true);
  //                         // Enable the interrupt. Parameters indicate
  //                         //  which axis to sample, when to trigger
  //                         //  (in this case, when the absolute mag
  //                         //  of the signal exceeds the threshold),
  //                         //  which interrupt source we're configuring,
  //                         //  and whether to enable (true) or disable
  //                         //  (false) the interrupt.

  
}

void loop() 
{
  static long loopTimer = 0;
  int16_t x, y, z;
  if (millis() - loopTimer > 1000)
  {
    loopTimer = millis();
    xl.readAxes(x, y, z);  // The readAxes() function transfers the
                           //  current axis readings into the three
                           //  parameter variables passed to it.
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    Serial.println(xl.convertToG(6,x)); // The convertToG() function
    Serial.println(xl.convertToG(6,y)); // accepts as parameters the
    Serial.println(xl.convertToG(6,z)); // raw value and the current
    Serial.println(" ");                // maximum g-rating.
  }
  // if (digitalRead(9) == HIGH)
  // {
  //   Serial.println("Interrupt");
  // }
}