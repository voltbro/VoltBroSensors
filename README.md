
## Набор библиотек для работы с датчиками модуля GY-801 (10DOF + BMP180) ##

Единый API для работы со всеми датчиками. Библиотеки проверены для платы IMU GY-801.

### Установка библиотеки ###

Библиотеку необходимо скачать [VoltBroSensors.zip](https://github.com/voltbro/VoltBroSensors/archive/master.zip),
и распаковать в директорию Arduino/libraries/.

Инструкция для установки библиотеки https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use

## Датчики ##
Список поддерживаемых датчиков

**Акселерометр**

  - [ADXL345](http://github.com/voltbro/VoltBroSensors/tree/master/examples/ADXL345_measure/ADXL345_measure.ino) (i2c IMU GY-801)

**Гироскоп**

  - [L3G4200D](http://github.com/voltbro/VoltBroSensors/tree/master/examples/L3G4200D_measure/L3G4200D_measure.ino) (i2c IMU GY-801)

**Магнитометр**

  - [HMC5883L](http://github.com/voltbro/VoltBroSensors/tree/master/examples/HMC5883L_measure/HMC5883L_measure.ino) (i2c IMU GY-801)

**Давления**

  - [BMP180](http://github.com/voltbro/VoltBroSensors/tree/master/examples/BMP180_measure/BMP180_measure.ino) (i2c IMU GY-801)


### Примем использования ###

```
#include <VB_BMP180.h>
VB_BMP180 barometer;

barometer_connection = barometer.begin();
if (barometer_connection) {
      barometer.read();
      Serial.println(barometer.pres);
}    

```

Посмотреть другие [примеры](http://github.com/voltbro/VoltBroSensors/tree/master/examples/) для других датчиков.


### Авторы ###

Библиотека поддерживается группой [БратьяВольт](http://voltbro.com).

Основано на библиотеке созданной Владимиром Мединским [medinskiy.net](http://medinskiy.net) для проекта [CanSat Junior](http://roscansat.com)
