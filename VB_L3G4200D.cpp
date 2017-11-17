#include "VB_L3G4200D.h"
#include "VoltBroSensors.h"

// Если адрес датчика не указан, используем адрес по-умолчанию.
//
// см L3G4200D_DEFAULT_ADDRESS
//для платы GY801 всегда используем такой адрес
//Инициализация с адресом по умолчанию
bool VB_L3G4200D::begin() {
    return begin(L3G4200D_DEFAULT_ADDRESS);
}

//Инициализация с адресом
bool VB_L3G4200D::begin(uint8_t address) {
    Wire.begin();
    dev_addr = address;
    if (testConnection()) {
        initialize(default_scale);
        return true;
    } else {
        return false;
    }
}

//Установка основных настроек и включение
//
void VB_L3G4200D::initialize(uint8_t scale) {
    VoltBroSensors::I2C_WriteReg(dev_addr, L3G4200D_RA_CTRL_REG1, L3G4200D_RATE_100HZ_BW12_5 | L3G4200D_ENABLE_ALL | L3G4200D_POWER_ON);

    uint8_t init_scale = L3G4200D_FULL_SCALE_500;
    dpsPerDigit        = L3G4200D_DPS_500;

    switch (scale) {
        case L3G4200D_FULL_SCALE_250:
            init_scale  = L3G4200D_FULL_SCALE_250;
            dpsPerDigit = L3G4200D_DPS_250;
            break;
        case L3G4200D_FULL_SCALE_500:
            init_scale  = L3G4200D_FULL_SCALE_500;
            dpsPerDigit = L3G4200D_DPS_500;
            break;
        case L3G4200D_FULL_SCALE_2000:
            init_scale  = L3G4200D_FULL_SCALE_2000;
            dpsPerDigit = L3G4200D_DPS_2000;
            break;
        default:
            break;
    }

    VoltBroSensors::I2C_WriteReg(dev_addr, L3G4200D_RA_CTRL_REG4, init_scale);
}

// Проверка соединения с девайсом
bool VB_L3G4200D::testConnection() {
    VoltBroSensors::I2C_ReadBytes(dev_addr, L3G4200D_RA_WHO_AM_I, 1, buffer);
    return buffer[0] == 0xD3;
}

// Данные произведенных измерений по трем осям (использовать  только
//этот метод, когда надо получить значения по всем трем осям в один момент времени)
void VB_L3G4200D::readRaw() {
    uint8_t MSB, LSB;

    MSB   = VoltBroSensors::I2C_getRegister(dev_addr, 0x29, 1);
    LSB   = VoltBroSensors::I2C_getRegister(dev_addr, 0x28, 1);
    x_raw = ((MSB << 8) | LSB);

    MSB   = VoltBroSensors::I2C_getRegister(dev_addr, 0x2B, 1);
    LSB   = VoltBroSensors::I2C_getRegister(dev_addr, 0x2A, 1);
    y_raw = ((MSB << 8) | LSB);

    MSB   = VoltBroSensors::I2C_getRegister(dev_addr, 0x2D, 1);
    LSB   = VoltBroSensors::I2C_getRegister(dev_addr, 0x2C, 1);
    z_raw = ((MSB << 8) | LSB);
}

void VB_L3G4200D::read() {
    readRaw();

    x = (float)x_raw * dpsPerDigit;
    y = (float)y_raw * dpsPerDigit;
    z = (float)z_raw * dpsPerDigit;
}
