#ifndef _VoltBroSensors_H_
#define _VoltBroSensors_H_

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "Wire.h"

#define VB_SENSORS_VERSION 1

class VoltBroSensors
{
  public:
    static void I2C_WriteReg(uint8_t dev_addr, uint8_t register_addr, uint8_t data);
    static void I2C_ReadBytes(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t *buffer);
    static void I2C_writeRegister8(uint8_t dev_addr, uint8_t register_addr, uint8_t value);
    static uint8_t I2C_fastRegister8(uint8_t dev_addr, uint8_t register_addr);
    static int32_t I2C_getRegister(uint8_t dev_addr, uint8_t register_addr, uint8_t num, bool unsign = true);
};

#endif /* _VoltBroSensors_H_*/
