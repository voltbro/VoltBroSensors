#include "VB_ADXL346.h"
#include "VoltBroSensors.h"
#include <Arduino.h>

// Если адрес датчика не указан, используем адрес по-умолчанию.
//
// см ADXL346_DEFAULT_ADDRESS
// для платы GY801 всегда используем такой адрес
// Инициализация с адресом по умолчанию
bool VB_ADXL346::begin() {
    return begin(ADXL346_DEFAULT_ADDRESS);
}

// Инициализация с адресом
bool VB_ADXL346::begin(uint8_t address) {
    Wire.begin();
    dev_addr = address;
    if (testConnection()) {
        initialize();
        return true;
    } else {
        return false;
    }
}

// Конфигурация регистра DATA_FORMAT
// +-16G 256/g, 13bit
void VB_ADXL346::Data_Format_Config() {
    uint8_t conf = ADXL346_SELF_TEST_OFF;

    conf |= ADXL346_SPI_OFF | ADXL346_INT_INVERT_OFF | ADXL346_FULL_RES_ON //full resolution
            | ADXL346_JUSTIFY_OFF | ADXL346_RANGE_16G;                     //+-16g

    VoltBroSensors::I2C_WriteReg(dev_addr, ADXL346_RA_DATA_FORMAT, conf);
}

void VB_ADXL346::PowerOn() {
    uint8_t power = ADXL346_AUTO_SLEEP_OFF; //автоматически не засыпаем
    power |= ADXL346_MEASURE_ON             //начать измерения
             | ADXL346_SLEEP_OFF            //вообще не спать
             | ADXL346_WAKEUP_8HZ; //выходить из сна 8 раз в секунду, не актуально при указании предыдущих настроек

    VoltBroSensors::I2C_WriteReg(dev_addr, ADXL346_RA_POWER_CTL, power);
}

// Установка основных настроек и включение
void VB_ADXL346::initialize() {
    VoltBroSensors::I2C_WriteReg(dev_addr, ADXL346_RA_BW_RATE,
                                 ADXL346_RATE_25); //Частота измерений 25Hz, Потребление обычное
    Data_Format_Config();
    PowerOn();
}

// Проверка соединения с девайсом
bool VB_ADXL346::testConnection() {
    VoltBroSensors::I2C_ReadBytes(dev_addr, ADXL346_RA_DEVID, 1, buffer);
    return buffer[0] == 0xE6;
}

// Регистры сдвига
// Получить информацию об установленных сдвигах
void VB_ADXL346::getOffset(int8_t *x, int8_t *y, int8_t *z) {
    VoltBroSensors::I2C_ReadBytes(dev_addr, ADXL346_RA_OFSX, 3, buffer);
    *x = buffer[0];
    *y = buffer[1];
    *z = buffer[2];
}

// Установить сдвиги в регистр
void VB_ADXL346::setOffset(int8_t x, int8_t y, int8_t z) {
    VoltBroSensors::I2C_WriteReg(dev_addr, ADXL346_RA_OFSX, x);
    VoltBroSensors::I2C_WriteReg(dev_addr, ADXL346_RA_OFSY, y);
    VoltBroSensors::I2C_WriteReg(dev_addr, ADXL346_RA_OFSZ, z);
}

// Cырые Данные произведенных измерений по трем осям
void VB_ADXL346::readRaw() {
    VoltBroSensors::I2C_ReadBytes(dev_addr, ADXL346_RA_DATAX0, 6, buffer);
    x_raw = (((int16_t)buffer[1]) << 8) | buffer[0];
    y_raw = (((int16_t)buffer[3]) << 8) | buffer[2];
    z_raw = (((int16_t)buffer[5]) << 8) | buffer[4];
}

void VB_ADXL346::read() {
    readRaw();
    x = x_raw * 0.03828125f; // 9.8/256.0f;
    y = y_raw * 0.03828125f;
    z = z_raw * 0.03828125f;
}
