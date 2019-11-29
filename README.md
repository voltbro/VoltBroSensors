## Arduino libraries for ADXL345, ADXL346, L3G4200D, HMC5883L, BMP180 ##

 - Arduino library ADXL345 accelerometr
 - Arduino library ADXL346 accelerometr
 - Arduino library L3G4200D gyro
 - Arduino library HMC5883L magnetic sensor
 - Arduino library BMP180 pressure sensor
 - Arduino library BMP280 pressure sensor
 - Arduino library MPU9250 IMU
 
## Авторы ##

Библиотека поддерживается группой [БратьяВольт](http://voltbro.ru) в рамках чемпионата "Реактивное Движение" (http://gorocket.ru/)

Основано на библиотеке созданной Владимиром Мединским [medinskiy.net](http://medinskiy.net)

## Набор библиотек для работы с датчиками модуля GY-801 (10DOF + BMP180) ##

Единый API для работы со всеми датчиками. Библиотеки проверены для платы IMU GY-801.
Добавленна поддержка ADXL346  


### Установка библиотеки ###

Библиотеку необходимо скачать [VoltBroSensors.zip](https://github.com/voltbro/VoltBroSensors/archive/master.zip),
и распаковать в директорию Arduino/libraries/.

Инструкция для установки библиотеки https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use

## Датчики ##
Список поддерживаемых датчиков

**Акселерометр**

  - [ADXL345](http://github.com/voltbro/VoltBroSensors/tree/master/examples/ADXL345_measure/ADXL345_measure.ino) (I2C IMU GY-801)
  - [ADXL346](http://github.com/voltbro/VoltBroSensors/tree/master/examples/ADXL346_measure/ADXL346_measure.ino) (I2C)
  
**Гироскоп**

  - [L3G4200D](http://github.com/voltbro/VoltBroSensors/tree/master/examples/L3G4200D_measure/L3G4200D_measure.ino) (I2C IMU GY-801)

**Магнитометр**

  - [HMC5883L](http://github.com/voltbro/VoltBroSensors/tree/master/examples/HMC5883L_measure/HMC5883L_measure.ino) (I2C IMU GY-801)

**Давления**

  - [BMP180](http://github.com/voltbro/VoltBroSensors/tree/master/examples/BMP180_measure/BMP180_measure.ino) (I2C IMU GY-801)
  - [BMP280](http://github.com/voltbro/VoltBroSensors/tree/master/examples/BMP280_measure/BMP280_measure.ino) (I2C IMU GY-91)

**IMU**
  - [MPU9250](http://github.com/voltbro/VoltBroSensors/tree/master//examples/MPU9250_Basic)

**Плата GY91**
  - [GY91](http://github.com/voltbro/VoltBroSensors/tree/master/examples/GY91_measure)
  

### Пример использования для Arduino ###

```
#include <VB_BMP180.h>
VB_BMP180 barometer;
boolean barometer_connection;

void setup() {
  barometer_connection = barometer.begin();
}

void loop() {
  if (barometer_connection) {
    barometer.read();
    Serial.println(barometer.temp);
  }
}
```

Посмотреть другие [примеры](http://github.com/voltbro/VoltBroSensors/tree/master/examples/) для датчиков.
