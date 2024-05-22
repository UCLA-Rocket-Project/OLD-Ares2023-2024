#ifndef ADS_H
#define ADS_H
#include <Arduino.h>
#include "HAL.h"
#include "Libraries/ADS1256/RP_ADS1256.h"

namespace ADS {

    long rawConversion = 0; //24-bit raw value
    float voltageValue = 0; //human-readable floating point value

    int singleEndedChannels[8] = {SING_0, SING_1, SING_2, SING_3, SING_4, SING_5, SING_6, SING_7}; //Array to store the single-ended channels
    int differentialChannels[4] = {DIFF_0_1, DIFF_2_3, DIFF_4_5, DIFF_6_7}; //Array to store the differential channels
    int inputChannel = 0; //Number used to pick the channel from the above two arrays
    char inputMode = ' '; //can be 's' and 'd': single-ended and differential

    int pgaValues[7] = {PGA_1, PGA_2, PGA_4, PGA_8, PGA_16, PGA_32, PGA_64}; //Array to store the PGA settings
    int pgaSelection = 0; //Number used to pick the PGA value from the above array

    int drateValues[16] =
    {
    DRATE_30000SPS,
    DRATE_15000SPS,
    DRATE_7500SPS,
    DRATE_3750SPS,
    DRATE_2000SPS,
    DRATE_1000SPS,
    DRATE_500SPS,
    DRATE_100SPS,
    DRATE_60SPS,
    DRATE_50SPS,
    DRATE_30SPS,
    DRATE_25SPS,
    DRATE_15SPS,
    DRATE_10SPS,
    DRATE_5SPS,
    DRATE_2SPS
    }; //Array to store the sampling rates

    int drateSelection = 0; //Number used to pick the sampling rate from the above array

    String registers[11] =
    {
    "STATUS",
    "MUX",
    "ADCON",
    "DRATE",
    "IO",
    "OFC0",
    "OFC1",
    "OFC2",
    "FSC0",
    "FSC1",
    "FSC2"
    };//Array to store the registers

    int registerToRead = 0; //Register number to be read
    int registerToWrite = 0; //Register number to be written
    int registerValueToWrite = 0; //Value to be written in the selected register

    float A_vals_GND[4] = {377.3854375, 377.5595104, 373.5603094, 3011.981888};
    float B_vals_GND[4] = {-254.1467792, -245.7410574, -253.5007837,-35.21335546};

    float A_vals_BAT[4] = {379.5779242, 378.3120511, 	376.2891849, 	3028.588266};
    float B_vals_BAT[4] = {-254.1467792,	-245.7410574,	-253.5007837,	-35.21335546};

    float ADS_readings[4] = {0};

    const float VREF = 2.500;

    ADS1256 A(HAL::ADS_DRDY, 0, 0, HAL::ADS_CS, VREF);

    void setupADS() {
        DEBUGLN("before setup");
        A.InitializeADC(HAL::sensorSPI); //See the documentation for every details
        DEBUGLN("after setup");

        //Setting up CS, RESET, SYNC and SPI
        //Assigning default values to: STATUS, MUX, ADCON, DRATE
        //Performing a SYSCAL

        //Below is a demonstration to change the values through the built-on functions of the library
        //Set a PGA value
        A.setPGA(PGA_1);  //0b00000000 - DEC: 0
        //--------------------------------------------

        //Set input channels
        A.setMUX(DIFF_0_1); //0b01100111 - DEC: 103
        //--------------------------------------------

        //Set DRATE
        A.setDRATE(DRATE_1000SPS); //0b00010011 - DEC: 19
        //--------------------------------------------

        //Read back the above 3 values to check if the writing was succesful
        DEBUG("PGA: ");
        DEBUGLN(A.readRegister(ADCON_REG));
        delay(100);
        //--
        DEBUG("MUX: ");
        DEBUGLN(A.readRegister(MUX_REG));
        delay(100);
        //--
        DEBUG("DRATE: ");
        DEBUGLN(A.readRegister(DRATE_REG));
        delay(100);
    }

    void readADS() {

        for (int i = 0; i < 4; i++)
        {
            ADS_readings[i] = A.convertToVoltage(A.cycleSingle()); //print the converted single-ended results with 4 digits
            DEBUG(String(ADS_readings[i],4));
            DEBUG(",");
        }
        for (int i = 0; i < 4; i++) {
            A.cycleSingle();
        }
        DEBUGLN();//Printing a linebreak - this will put the next 8 conversions in a new line
        
        A.stopConversion();
    }

};


#endif