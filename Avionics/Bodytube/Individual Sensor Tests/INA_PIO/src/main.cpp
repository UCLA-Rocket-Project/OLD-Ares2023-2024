#include <Arduino.h>

#include <Wire.h>
#include <infinityPV_INA233.h>

//I2C Adresses for the 2 INA233 on board 
INA233 ina(0x40);  // IC1 address 0x45 1000101 

void setup() {
  Serial.begin(9600);

  ina.begin();

  float R_shunt_IC1=0.01;   //call RS2 on the board 
  float I_max_IC1=2;

  //variables to catch the outputs from set_Calibration()
  uint16_t CAL=0;
  int16_t m_c=0;
  int16_t m_p=0;
  int8_t R_c=0;
  int8_t R_p=0;
  uint8_t Set_ERROR=0;
  float Current_LSB=0;
  float Power_LSB=0;
  //variable to check the loaded calibration
  uint16_t Read_CAL=0;

  CAL=ina.setCalibration(R_shunt_IC1,I_max_IC1,&Current_LSB,&Power_LSB,&m_c,&R_c,&m_p,&R_p,&Set_ERROR);
  ina.wireWriteByte (MFR_DEVICE_CONFIG, 0x06);
}

void loop() {
  float bus_voltage = 0;
  float shunt_voltage = 0;
  float current_value = 0;
  float power_value = 0;

  bus_voltage=ina.getBusVoltage_V();  //getting bus voltage (V) at INA233 IC1

  shunt_voltage=ina.getShuntVoltage_mV();  //getting shunt voltage (mV) at INA233 IC1

  current_value = ina.getCurrent_mA();

  power_value = ina.getAv_Power_mW();
  
  // Serial.print("Bus Voltage:   "); Serial.print(bus_voltage);Serial.print(" V, ");
  // Serial.print("Shunt voltage :   "); Serial.print(shunt_voltage);Serial.print(" mV, ");
  // Serial.print("Current draw :   "); Serial.print(shunt_voltage / 10e-3);Serial.print(" mA, ");
  // Serial.print("Current draw :   "); Serial.print(current_value);Serial.print(" mA, ");
  // Serial.print("Power draw :   "); Serial.print(power_value);Serial.println(" mW");
  Serial.print(bus_voltage);Serial.print(",");
  Serial.print(shunt_voltage);Serial.print(",");
  Serial.print(shunt_voltage / 10e-3);Serial.print(",");
  Serial.print(current_value);Serial.print(",");
  Serial.print(current_value * bus_voltage);Serial.print(",");
  Serial.print(power_value);Serial.print(",");
  Serial.print(power_value / shunt_voltage * bus_voltage);Serial.print(",");
  Serial.println();
  delay(2000);

}