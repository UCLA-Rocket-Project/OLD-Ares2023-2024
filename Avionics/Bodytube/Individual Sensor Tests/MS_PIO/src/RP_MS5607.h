#ifndef RP_MS5607_H
#define RP_MS5607_H

#include <Arduino.h>
#include <SPI.h>

class RP_MS5607 {
private:

    SPIClass* MS_SPI;
    uint8_t CS_Pin;

    uint8_t  _address;
    uint8_t  _samplingRate;
    int32_t  _temperature;
    int32_t  _pressure;
    float    _pressureOffset;
    float    _temperatureOffset;
    int      _result;
    float    C[7];
    uint32_t _lastRead;
    uint32_t _deviceID;
    bool     _compensation;

    int32_t c1, c2, c3, c4, c5, c6;



    enum COMMANDS {
        RESET     = 0x1E,
        ADC_READ  = 0x00,
        ADC_CONV  = 0x40,
        ADC_D1    = 0x00,
        ADC_D2    = 0x10,
        OSR_256   = 0x00,
        OSR_512   = 0x02,
        OSR_1024  = 0x04,
        OSR_2048  = 0x06,
        OSR_4096  = 0x08,
        PROM_READ = 0xA0
    };

    // datasheet page 10
    #define CMD_READ_ADC       0x00
    #define CMD_READ_PROM      0xA0
    #define CMD_RESET          0x1E
    #define CMD_CONVERT_D1     0x40
    #define CMD_CONVERT_D2     0x50

    SPISettings _spi_settings;

    void convert(const uint8_t addr, uint8_t bits);

public:

    RP_MS5607();
    void init(uint8_t cs, SPIClass* spi = &SPI);

    void writeCommand(const uint8_t command, int ms = 0);
    uint16_t readPROM(uint8_t address);
    uint32_t readADC(int command);

    void readCalibrationValues();

    void initConstants();

    bool readDeviceID();

    uint32_t getDeviceID() { return _deviceID; }

    uint32_t getRawTemperature() { return readADC(ADC_D2 | OSR_4096); }

    uint32_t getRawPressure() { return readADC(ADC_D1 | OSR_4096); }

    float getPressure();

    int performReading(uint8_t bits);

    float toAltitude(int pressure) {
        // Ref. 29124-AltimeterAppNote1.pdf
        const float R = 287.052; // specific gas constant R*/M0
        const float g = 9.80665; // standard gravity 
        const float t_grad = 0.0065; // gradient of temperature
        const float t0 = 273.15 + 15; // temperature at 0 altitude
        const float p0 = 101325; // pressure at 0 altitude

        return t0 / t_grad * (1 - exp((t_grad * R / g) * log(pressure / p0)));
    }
};

#endif