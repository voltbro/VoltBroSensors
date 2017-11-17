#ifndef _VB_L3G4200D_H_
#define _VB_L3G4200D_H_

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "Wire.h"

// clang-format off
#define L3G4200D_ADDRESS_HIGH 0x69 //I2C address of the L3G4200D
#define L3G4200D_ADDRESS_LOW 0x68
#define L3G4200D_DEFAULT_ADDRESS     L3G4200D_ADDRESS_HIGH

//Адреса регистров
#define L3G4200D_RA_WHO_AM_I  0x0F
#define L3G4200D_RA_CTRL_REG1 0x20
#define L3G4200D_RA_CTRL_REG2 0x21
#define L3G4200D_RA_CTRL_REG3 0x22
#define L3G4200D_RA_CTRL_REG4 0x23
#define L3G4200D_RA_CTRL_REG5 0x24

#define L3G4200D_RA_OUT_X_L 0x28


#define L3G4200D_RATE_100HZ_BW12_5          0b00000000//будем использовать это остальное для справки и RTFM
#define L3G4200D_RATE_100HZ_BW25            0b00100000
#define L3G4200D_RATE_200HZ_BW12_5          0b01000000
#define L3G4200D_RATE_200HZ_BW25            0b01010000
#define L3G4200D_RATE_400HZ                 0b10000000
#define L3G4200D_RATE_800HZ                 0b11000000

#define L3G4200D_ENABLE_X                   0b00000001
#define L3G4200D_ENABLE_Y                   0b00000010
#define L3G4200D_ENABLE_Z                   0b00000100
#define L3G4200D_POWER_ON                   0b00001000
#define L3G4200D_ENABLE_ALL                 0b00000111
#define L3G4200D_DISABLE_ALL_POWER_OFF      0b00000000

#define L3G4200D_FULL_SCALE_250             0b00000000
#define L3G4200D_FULL_SCALE_500             0b00010000
#define L3G4200D_FULL_SCALE_2000            0b00100000
#define L3G4200D_DPS_250                    .00875f
#define L3G4200D_DPS_500                    .0175f
#define L3G4200D_DPS_2000                   .07f
// clang-format on

class VB_L3G4200D
{
  public:
    bool begin();
    bool begin(uint8_t address);

    // DATA* registers
    void readRaw();
    void read();

    //Output Data
    int16_t x_raw, y_raw, z_raw;
    float x, y, z;

    //Init data
    uint8_t default_scale = L3G4200D_FULL_SCALE_500;

  private:
    uint8_t dev_addr;
    uint8_t buffer[6];
    float dpsPerDigit;
    void initialize(uint8_t scale);
    bool testConnection();
};

#endif /* _VB_L3G4200D_H_ */
