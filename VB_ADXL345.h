#ifndef _VB_ADXL345_H_
#define _VB_ADXL345_H_

#include "Wire.h"

//I2C адрес акселерометра
//Если пин акселерометра ALT ADDR(12 )
#define ADXL345_ADDRESS_ALT_LOW 0x53  //Если пин акселерометра ALT ADDR(12) подключен к (GND), как в модуле GY801
#define ADXL345_ADDRESS_ALT_HIGH 0x1D //Если пин акселерометра ALT ADDR(12) подключен к (VCC)
#define ADXL345_DEFAULT_ADDRESS ADXL345_ADDRESS_ALT_LOW

//Адреса регистров
#define ADXL345_RA_DEVID 0x00
#define ADXL345_RA_RESERVED1 0x01
#define ADXL345_RA_THRESH_TAP 0x1D
#define ADXL345_RA_OFSX 0x1E
#define ADXL345_RA_OFSY 0x1F
#define ADXL345_RA_OFSZ 0x20
#define ADXL345_RA_DUR 0x21
#define ADXL345_RA_LATENT 0x22
#define ADXL345_RA_WINDOW 0x23
#define ADXL345_RA_THRESH_ACT 0x24
#define ADXL345_RA_THRESH_INACT 0x25
#define ADXL345_RA_TIME_INACT 0x26
#define ADXL345_RA_ACT_INACT_CTL 0x27
#define ADXL345_RA_THRESH_FF 0x28
#define ADXL345_RA_TIME_FF 0x29
#define ADXL345_RA_TAP_AXES 0x2A
#define ADXL345_RA_ACT_TAP_STATUS 0x2B
#define ADXL345_RA_BW_RATE 0x2C
#define ADXL345_RA_POWER_CTL 0x2D
#define ADXL345_RA_INT_ENABLE 0x2E
#define ADXL345_RA_INT_MAP 0x2F
#define ADXL345_RA_INT_SOURCE 0x30
#define ADXL345_RA_DATA_FORMAT 0x31
#define ADXL345_RA_DATAX0 0x32
#define ADXL345_RA_DATAX1 0x33
#define ADXL345_RA_DATAY0 0x34
#define ADXL345_RA_DATAY1 0x35
#define ADXL345_RA_DATAZ0 0x36
#define ADXL345_RA_DATAZ1 0x37
#define ADXL345_RA_FIFO_CTL 0x38
#define ADXL345_RA_FIFO_STATUS 0x39

#define ADXL345_RATE_3200 0b1111
#define ADXL345_RATE_1600 0b1110
#define ADXL345_RATE_800 0b1101
#define ADXL345_RATE_400 0b1100
#define ADXL345_RATE_200 0b1011
#define ADXL345_RATE_100 0b1010
#define ADXL345_RATE_50 0b1001
#define ADXL345_RATE_25 0b1000
#define ADXL345_RATE_12P5 0b0111
#define ADXL345_RATE_6P25 0b0110
#define ADXL345_RATE_3P13 0b0101
#define ADXL345_RATE_1P56 0b0100
#define ADXL345_RATE_0P78 0b0011
#define ADXL345_RATE_0P39 0b0010
#define ADXL345_RATE_0P20 0b0001
#define ADXL345_RATE_0P10 0b0000

#define ADXL345_WAKEUP_8HZ 0b00
#define ADXL345_WAKEUP_4HZ 0b01
#define ADXL345_WAKEUP_2HZ 0b10
#define ADXL345_WAKEUP_1HZ 0b11

/** SELF Test ON */
#define ADXL345_SELF_TEST_ON 0b10000000
/** SELF Test OFF */
#define ADXL345_SELF_TEST_OFF 0b00000000
/** SELF SPI ON */
#define ADXL345_SPI_ON 0b01000000
/** SELF SPI OFF */
#define ADXL345_SPI_OFF 0b00000000
/** INT_INVERT ON */
#define ADXL345_INT_INVERT_ON 0b00100000
/** INT_INVERT OFF */
#define ADXL345_INT_INVERT_OFF 0b00000000
/** FULL_RES ON */
#define ADXL345_FULL_RES_ON 0b00001000
/** FULL_RES OFF */
#define ADXL345_FULL_RES_OFF 0b00000000
/** JUSTIFY ON */
#define ADXL345_JUSTIFY_ON 0b00000100
/** JUSTIFY OFF */
#define ADXL345_JUSTIFY_OFF 0b00000000

#define ADXL345_RANGE_2G 0b00
#define ADXL345_RANGE_4G 0b01
#define ADXL345_RANGE_8G 0b10
#define ADXL345_RANGE_16G 0b11

/** AUTO SLEEP ON */
#define ADXL345_AUTO_SLEEP_ON 0b00010000
/** AUTO SLEEP OFF */
#define ADXL345_AUTO_SLEEP_OFF 0b00000000
/** AUTO MEASURE ON */
#define ADXL345_MEASURE_ON 0b00001000
/** AUTO MEASURE OFF */
#define ADXL345_MEASURE_OFF 0b00000000
/** SLEEP ON */
#define ADXL345_SLEEP_ON 0b00000100
/** SLEEP OFF */
#define ADXL345_SLEEP_OFF 0b00000000

/** WAKEUP 8Hz */
#define ADXL345_WAKEUP_8HZ 0b00
/** WAKEUP 4Hz */
#define ADXL345_WAKEUP_4HZ 0b01
/** WAKEUP 2Hz */
#define ADXL345_WAKEUP_2HZ 0b10
/** WAKEUP 1Hz */
#define ADXL345_WAKEUP_1HZ 0b11

class VB_ADXL345
{
  public:
    bool begin();
    bool begin(uint8_t address);

    // OFS* registers
    void getOffset(int8_t *x, int8_t *y, int8_t *z);
    void setOffset(int8_t x, int8_t y, int8_t z);

    // Read DATA
    void readRaw();
    void read();

    //Output Data
    int16_t x_raw, y_raw, z_raw;
    float x, y, z;

  private:
    void initialize();
    bool testConnection();
    void Data_Format_Config();
    void PowerOn();

    uint8_t dev_addr;
    uint8_t buffer[6];
};

#endif /* _VB_ADXL345_H_ */
