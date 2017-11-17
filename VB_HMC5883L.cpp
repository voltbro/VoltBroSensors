#include "VB_HMC5883L.h"
#include "VoltBroSensors.h"
#include <Arduino.h>

bool VB_HMC5883L::begin() {
    return begin(HMC5883L_ADDRESS);
}

bool VB_HMC5883L::begin(uint8_t address) {
    Wire.begin();
    dev_addr = address;

    if (testConnection()) {
        initialize();
        return true;
    } else {
        return false;
    }
}

bool VB_HMC5883L::testConnection() {
    if ((VoltBroSensors::I2C_getRegister(dev_addr, HMC5883L_RA_IDENT_A, 1) != 0x48) ||
        (VoltBroSensors::I2C_getRegister(dev_addr, HMC5883L_RA_IDENT_B, 1) != 0x34) ||
        (VoltBroSensors::I2C_getRegister(dev_addr, HMC5883L_RA_IDENT_C, 1) != 0x33)) {
        return false;
    }

    return true;
}

void VB_HMC5883L::initialize() {
    //set range
    VoltBroSensors::I2C_WriteReg(dev_addr, HMC5883L_RA_CONFIG_B, HMC5883L_RANGE_1_3GA << 5);
    mgPerDigit = HMC5883L_mgPerDigit_1_3GA;

    //set samples and datarate
    VoltBroSensors::I2C_WriteReg(dev_addr, HMC5883L_RA_CONFIG_A, HMC5883L_SAMPLES_1 << 5 | HMC5883L_DATARATE_15HZ << 2);
    //set mode
    VoltBroSensors::I2C_WriteReg(dev_addr, HMC5883L_RA_MODE, HMC5883L_CONTINOUS);
}

void VB_HMC5883L::readRaw() {
    VoltBroSensors::I2C_ReadBytes(dev_addr, HMC5883L_RA_OUT_X_M, 6, buffer);
    x_raw = (((int16_t)buffer[0]) << 8) | buffer[1];
    y_raw = (((int16_t)buffer[4]) << 8) | buffer[5];
    z_raw = (((int16_t)buffer[2]) << 8) | buffer[3];
}

void VB_HMC5883L::read() {
    readRaw();
    x = (float)x_raw * mgPerDigit;
    y = (float)y_raw * mgPerDigit;
    z = (float)z_raw * mgPerDigit;
}
