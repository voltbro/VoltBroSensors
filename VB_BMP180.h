#ifndef _VB_BMP180_H_
#define _VB_BMP180_H_

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "Wire.h"

#define BMP180_DEFAULT_ADDRESS 0x77
#define BMP180_RA_DEVID 0xD0

class VB_BMP180
{
  public:
    bool begin();
    bool begin(uint8_t address);

    // Read DATA
    // Чтение всех данных сенсора, с указанием точности OSS=0...3 (0-минимальная, 3-максимальная)
    boolean read(uint8_t OSS = 0);

    //Output Data
    float temp; //	Значение температуры в °C
    float pres; //	Значение давления в мм.рт.ст.
    float alti; //	Значение высоты над уровнем моря

    // Init vars
    float start_altitude = 0;

  private:
    bool testConnection();
    void initialize();       //	Инициализация сенсора
    void DelayFlagSCO(void); //	Задержка по состоянию флага преобразований SCO (5бит в регистре 0xF4)

    uint8_t dev_addr;
    uint8_t buffer[6];

    float SLP; //	Расчётное давление на уровне моря в мм.рт.ст.
    int32_t AC1 = 0, AC2 = 0, AC3 = 0, AC4 = 0, AC5 = 0, AC6 = 0, B_1 = 0, B_2 = 0, MB = 0, MC = 0, MD = 0, UT = 0, UP = 0, PP1 = 0, PP4 = 0;
    uint32_t PP2 = 0, PP3 = 0; //	Промежуточные переменные
    boolean ErrData = false;   //	Флаг ошибки чтения данных
};

#endif /* _VB_BMP180_H_ */
