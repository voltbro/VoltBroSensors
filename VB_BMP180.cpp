#include "VB_BMP180.h"
#include "VoltBroSensors.h"

bool VB_BMP180::begin() {
    return begin(BMP180_DEFAULT_ADDRESS);
}

bool VB_BMP180::begin(uint8_t address) {
    Wire.begin();
    dev_addr = address;
    if (testConnection()) {
        initialize();
        return true;
    } else {
        return false;
    }
}

void VB_BMP180::initialize() {
    // Читаем калибровочные коэффициенты (константы для корректировки показаний температуры и давления) из EEPROM
    AC1 = VoltBroSensors::I2C_getRegister(dev_addr, 0xAA, 2);
    AC2 = VoltBroSensors::I2C_getRegister(dev_addr, 0xAC, 2);
    AC3 = VoltBroSensors::I2C_getRegister(dev_addr, 0xAE, 2);
    AC4 = VoltBroSensors::I2C_getRegister(dev_addr, 0xB0, 2, false);
    AC5 = VoltBroSensors::I2C_getRegister(dev_addr, 0xB2, 2, false);
    AC6 = VoltBroSensors::I2C_getRegister(dev_addr, 0xB4, 2, false);
    B_1 = VoltBroSensors::I2C_getRegister(dev_addr, 0xB6, 2);
    B_2 = VoltBroSensors::I2C_getRegister(dev_addr, 0xB8, 2);
    MB  = VoltBroSensors::I2C_getRegister(dev_addr, 0xBA, 2);
    MC  = VoltBroSensors::I2C_getRegister(dev_addr, 0xBC, 2);
    MD  = VoltBroSensors::I2C_getRegister(dev_addr, 0xBE, 2);
    SLP = 0;

    // инициируем чтение для получения текущего давления
    read();

    // расчетное давление на уровне моря в мм.рт.ст.
    SLP = pres / pow(1 - (start_altitude / 44330), 5.255);
}

// Проверка соединения с девайсом
bool VB_BMP180::testConnection() {
    VoltBroSensors::I2C_ReadBytes(dev_addr, BMP180_RA_DEVID, 1, buffer);
    return buffer[0] == 0x55;
}

boolean VB_BMP180::read(uint8_t OSS) {
    VoltBroSensors::I2C_WriteReg(dev_addr, 0xF4, 0x2E);
    DelayFlagSCO();

    // Читаем "сырую" температуру:	записываем в регистр 0xF4 значение 0x2E, ждем флаг SCO,
    // читаем 2байта из регистров 0xF6 0xF7
    UT = VoltBroSensors::I2C_getRegister(dev_addr, 0xF6, 2);
    VoltBroSensors::I2C_WriteReg(dev_addr, 0xF4, 0x34 + (OSS << 6));
    DelayFlagSCO();

    // Читаем "сырое" давление:	записываем в регистр 0xF4 значение 0x34 или 0x74 или 0xB4 или 0xF4
    // (в зависимости от значения OSS), ждем флаг SCO, читаем 3байта из регистров 0xF6 0xF7 0xF8
    UP = VoltBroSensors::I2C_getRegister(dev_addr, 0xF6, 3) >> (8 - OSS);
    if (ErrData) {
        return false;
    }

    // Расчёт промежуточных переменных
    PP1 = ((UT - AC6) * AC5 >> 15) + (MC << 11) / (((UT - AC6) * AC5 >> 15) + MD);
    PP2 = ((uint32_t)AC4 * (uint32_t)(((((AC3 * (PP1 - 4000)) >> 13) + ((B_1 * (((PP1 - 4000) * (PP1 - 4000)) >> 12)) >> 16) + 2) >> 2) + 32768)) >> 15;
    PP3 = ((uint32_t)UP - ((((AC1 * 4 + ((B_2 * (((PP1 - 4000) * (PP1 - 4000)) >> 12)) >> 11) + ((AC2 * (PP1 - 4000)) >> 11)) << OSS) + 2) >> 2)) *
          (uint32_t)(50000UL >> OSS);
    PP4 = PP3 < 0x80000000 ? PP3 * 2 / PP2 : PP3 / PP2 * 2;

    // Расчет температуры, давления, высоты
    temp = ((float)PP1 + 8) / 160;
    pres = (PP4 + (((((PP4 >> 8) * (PP4 >> 8) * 3038) >> 16) + ((-7357 * PP4) >> 16) + 3791) >> 4)) / 133.322;
    alti = 44330 * (1 - pow(pres / SLP, 1 / 5.255));
    return true;
}

void VB_BMP180::DelayFlagSCO(void) {
    int i = 0;
    while (VoltBroSensors::I2C_getRegister(dev_addr, 0xF4, 1) & 0x20 && i < 50) {
        delay(1);
        i++;
    }
    if (i >= 50 || i == 0) {
        ErrData = true;
    } //	Выходим из цикла если 5й бит регистра 0xF4 (флаг SCO) = 0, или после 50го цикла, или если циклов не было
}
