#include <Arduino.h>
#include <SPI.h>
#include <RP_MS5607.h>

RP_MS5607::RP_MS5607() {}

void RP_MS5607::init(uint8_t cs,SPIClass* spi) {

    MS_SPI = spi;
    CS_Pin = cs;
    _deviceID = 0;
    pinMode(CS_Pin, OUTPUT);
    digitalWrite(CS_Pin, HIGH);

    _spi_settings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
    // Serial.println(spi == nullptr);
    writeCommand(CMD_RESET);

    initConstants();

}

void RP_MS5607::initConstants() {

//  constants that were multiplied in read() - datasheet page 8
//  do this once and you save CPU cycles
    C[0] = 1;
    C[1] = 65536L;        //  SENSt1   = C[1] * 2^16
    C[2] = 131072L;       //  OFFt1    = C[2] * 2^17
    C[3] = 7.8125E-3;     //  TCS      = C[3] / 2^7
    C[4] = 1.5625e-2;     //  TCO      = C[4] / 2^6
    C[5] = 256;             //  Tref     = C[5] * 2^8     |    * 2^8
    C[6] = 1.1920928955E-7; //  TEMPSENS = C[6] / 2^23    |    / 2^23
}

void RP_MS5607::readCalibrationValues() {
    c1 = readPROM(1);
    c2 = readPROM(2);
    c3 = readPROM(3);
    c4 = readPROM(4);
    c5 = readPROM(5);
    c6 = readPROM(6);

    // Serial.println(c1);
    // Serial.println(c2);
    // Serial.println(c3);
    // Serial.println(c4);
    // Serial.println(c5);
    // Serial.println(c6);
}

bool RP_MS5607::readDeviceID() {
    bool ROM_OK = true;
    for (uint8_t reg = 0; reg < 7; reg++)
    {
        //  used indices match datasheet.
        //  C[0] == manufacturer - read but not used;
        //  C[7] == CRC - skipped.
        uint16_t tmp = readPROM(reg);
        C[reg] *= tmp;
        //  _deviceID is a simple SHIFT XOR merge of PROM data
        _deviceID <<= 4;
        _deviceID ^= tmp;
        // Serial.println(readProm(reg));
        if (reg > 0)
        {
            ROM_OK = ROM_OK && (tmp != 0);
        }
    }
    return ROM_OK;
}

void RP_MS5607::writeCommand(const uint8_t command, int ms) {

    digitalWrite(CS_Pin, LOW);
    MS_SPI->beginTransaction(_spi_settings);

    MS_SPI->transfer(command);
    // if (ms) delayMicroseconds(ms);

    MS_SPI->endTransaction();
    digitalWrite(CS_Pin, HIGH);
}

uint16_t RP_MS5607::readPROM(uint8_t address) {

    uint8_t promCRCRegister = 7;
        if (address > promCRCRegister) return 0;

    uint16_t value = 0;
    digitalWrite(CS_Pin, LOW);
    MS_SPI->beginTransaction(_spi_settings);
    MS_SPI->transfer(CMD_READ_PROM + address * 2);
    value += MS_SPI->transfer(0x00);
    value <<= 8;
    value += MS_SPI->transfer(0x00);
    MS_SPI->endTransaction();
    digitalWrite(CS_Pin, HIGH);

    return value;
}

uint32_t RP_MS5607::readADC(int command) {
    digitalWrite(CS_Pin, LOW);
    MS_SPI->transfer(ADC_CONV | command);
    static int duration[] = {600, 1200, 2300, 4600, 9100};
    delayMicroseconds(duration[(command & 0x0F) >> 1]);
    digitalWrite(CS_Pin, HIGH);

    uint32_t value = 0;

    digitalWrite(CS_Pin, LOW);
    MS_SPI->write(ADC_READ);
    value += MS_SPI->transfer(0x00);
    value <<= 8;
    value += MS_SPI->transfer(0x00);
    value <<= 8;
    value += MS_SPI->transfer(0x00);
    digitalWrite(CS_Pin, HIGH);
    return value;
}

int RP_MS5607::performReading(uint8_t bits) {

//   float dT = _D2 - C[5];
//   _temperature = 2000 + dT * C[6];

//   float offset =  C[2] + dT * C[4];
//   float sens = C[1] + dT * C[3];

//   if (_compensation)
//   {
//     //  SECOND ORDER COMPENSATION - PAGE 8/20
//     //  COMMENT OUT < 2000 CORRECTION IF NOT NEEDED
//     //  NOTE TEMPERATURE IS IN 0.01 C
//     if (_temperature < 2000)
//     {
//       float T2 = dT * dT * 4.6566128731E-10;
//       float t = (_temperature - 2000) * (_temperature - 2000);
//       float offset2 = 2.5 * t;
//       float sens2 = 1.25 * t;
//       //  COMMENT OUT < -1500 CORRECTION IF NOT NEEDED
//       if (_temperature < -1500)
//       {
//         t = (_temperature + 1500) * (_temperature + 1500);
//         offset2 += 7 * t;
//         sens2 += 5.5 * t;
//       }
//       _temperature -= T2;
//       offset -= offset2;
//       sens -= sens2;
//     }
//     //  END SECOND ORDER COMPENSATION
//   }

//   _pressure = (_D1 * sens * 4.76837158205E-7 - offset) * 3.051757813E-5;

//   _lastRead = millis();
  return 1;
}

float RP_MS5607::getPressure() {
    int dT = getRawTemperature() - C[5];
    float temp = 2000 + dT * C[6];
    int64_t off = C[2] + dT * C[4];
    int64_t sens = C[1] + dT * C[3];

    // 2nd order temperature compensation
    if (temp < 2000) {
        float T2 = dT * dT * 4.6566126731E-10;
        float t = (temp - 2000) * (temp - 2000);
        int64_t off2 = 2.5 * t;
        int64_t sens2 = 1.25 * t;
        if (temp < -1500) {
            t = (temp - 1500) * (temp - 1500);
            off2 += 7 * t;
            sens2 += 5.5 * t;
        }
        temp -= T2;
        off -= off2;
        sens -= sens2;
    }
    
    return (getRawPressure() * sens * 4.76837158205E-7 - off) * 3.051757813E-5;
    // float((((int64_t) getRawPressure() * sens >> 21) - off) >> 15);
}