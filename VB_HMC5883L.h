#ifndef _VB_HMC5883L_H_
#define _VB_HMC5883L_H_

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <Wire.h>

// clang-format off
#define HMC5883L_ADDRESS             0x1E
#define HMC5883L_RA_CONFIG_A         0x00
#define HMC5883L_RA_CONFIG_B         0x01
#define HMC5883L_RA_MODE             0x02
#define HMC5883L_RA_OUT_X_M          0x03
#define HMC5883L_RA_OUT_X_L          0x04
#define HMC5883L_RA_OUT_Z_M          0x05
#define HMC5883L_RA_OUT_Z_L          0x06
#define HMC5883L_RA_OUT_Y_M          0x07
#define HMC5883L_RA_OUT_Y_L          0x08
#define HMC5883L_RA_STATUS           0x09
#define HMC5883L_RA_IDENT_A          0x0A
#define HMC5883L_RA_IDENT_B          0x0B
#define HMC5883L_RA_IDENT_C          0x0C

#define HMC5883L_SAMPLES_8    			 0b11
#define HMC5883L_SAMPLES_4    			 0b10
#define HMC5883L_SAMPLES_2   				 0b01
#define HMC5883L_SAMPLES_1  			   0b00

#define HMC5883L_DATARATE_75HZ       0b110
#define HMC5883L_DATARATE_30HZ       0b101
#define HMC5883L_DATARATE_15HZ       0b100
#define HMC5883L_DATARATE_7_5HZ      0b011
#define HMC5883L_DATARATE_3HZ        0b010
#define HMC5883L_DATARATE_1_5HZ      0b001
#define HMC5883L_DATARATE_0_75_HZ    0b000

#define HMC5883L_RANGE_8_1GA     0b111
#define HMC5883L_mgPerDigit_8_1GA  4.35f
#define HMC5883L_RANGE_5_6GA     0b110
#define HMC5883L_mgPerDigit_5_6GA  3.03f
#define HMC5883L_RANGE_4_7GA     0b101
#define HMC5883L_mgPerDigit_4_7GA  2.56f
#define HMC5883L_RANGE_4GA       0b100
#define HMC5883L_mgPerDigit_4GA  2.27f
#define HMC5883L_RANGE_2_5GA     0b011
#define HMC5883L_mgPerDigit_2_5GA  1.52f
#define HMC5883L_RANGE_1_9GA     0b010
#define HMC5883L_mgPerDigit_1_9GA  1.22f
#define HMC5883L_RANGE_1_3GA     0b001
#define HMC5883L_mgPerDigit_1_3GA  0.092f
#define HMC5883L_RANGE_0_88GA    0b000
#define HMC5883L_mgPerDigit_0_88GA  0.073f

#define HMC5883L_IDLE          0b10
#define HMC5883L_SINGLE        0b01
#define HMC5883L_CONTINOUS     0b00
// clang-format on

class VB_HMC5883L
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

  private:
    bool testConnection();
    void initialize();

    uint8_t dev_addr;
    uint8_t buffer[6];
    float mgPerDigit;
};
#endif /*VB__VB_HMC5883L_H_ */
