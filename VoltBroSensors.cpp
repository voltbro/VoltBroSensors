#include "VoltBroSensors.h"
#include <Arduino.h>

void VoltBroSensors::I2C_WriteReg(uint8_t dev_addr, uint8_t register_addr, uint8_t data) {
    Wire.beginTransmission(dev_addr);
    Wire.write(register_addr);
    Wire.write(data);
    Wire.endTransmission();
}

void VoltBroSensors::I2C_ReadBytes(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t *buffer) {
    uint8_t i;
    Wire.beginTransmission(dev_addr);
    Wire.write(register_addr);
    Wire.endTransmission();
    Wire.requestFrom(dev_addr, num);
    for (i = 0; i < num; i++) {
        *buffer = Wire.read();
        buffer++;
    }
}

// Write byte to register
void VoltBroSensors::I2C_writeRegister8(uint8_t dev_addr, uint8_t register_addr, uint8_t value) {
    Wire.beginTransmission(dev_addr);
    Wire.write(register_addr);
    Wire.write(value);
    Wire.endTransmission();
}

// Read byte to register
uint8_t VoltBroSensors::I2C_fastRegister8(uint8_t dev_addr, uint8_t register_addr) {
    uint8_t value;
    Wire.beginTransmission(dev_addr);
    Wire.write(register_addr);
    Wire.endTransmission();

    Wire.requestFrom(dev_addr, (uint8_t)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

int32_t VoltBroSensors::I2C_getRegister(uint8_t dev_addr, uint8_t register_addr, uint8_t num, bool unsign) {
    int32_t result32 = 0;
    int16_t result16 = 0;
    int8_t result8   = 0;

    Wire.beginTransmission(dev_addr);
    Wire.write(register_addr);
    Wire.endTransmission();
    Wire.requestFrom(dev_addr, num);
    for (int i = 0; i < num; i++) {
        result32 <<= 8;
        result32 += Wire.read();
    }

    //	добавляем знак (если Uns==true)
    if (unsign && num == 1) {
        result8  = result32;
        result32 = result8;
    }

    //	добавляем знак (если Uns==true)
    if (unsign && num == 2) {
        result16 = result32;
        result32 = result16;
    }
    return result32;
}
