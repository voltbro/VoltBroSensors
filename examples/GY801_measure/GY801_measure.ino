#include <VB_ADXL345.h>
#include <VB_BMP180.h>
#include <VB_HMC5883L.h>
#include <VB_L3G4200D.h>

VB_ADXL345 accel; //создание объекта типа акселерометр ADXL345
VB_L3G4200D gyro; //создание объекта типа гироскоп L3G4200D
VB_HMC5883L compass;
VB_BMP180 barometer;

bool gyro_connection;
bool accel_connection;
bool compass_connection;
bool barometer_connection;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Ожидаем когда появиться Serial Port через USB
    }
    setupSensors();
}

void loop() {
    if ((accel_connection && gyro_connection && compass_connection && barometer_connection)) {
        accel.read();
        Serial.print("accel:\t");
        Serial.print(accel.x);
        Serial.print("\t");
        Serial.print(accel.y);
        Serial.print("\t");
        Serial.println(accel.z);

        gyro.read();
        Serial.print("gyros:\t");
        Serial.print(gyro.x);
        Serial.print("\t");
        Serial.print(gyro.y);
        Serial.print("\t");
        Serial.println(gyro.z);

        compass.read();
        Serial.print("compa:\t");
        Serial.print(compass.x);
        Serial.print("\t");
        Serial.print(compass.y);
        Serial.print("\t");
        Serial.println(compass.z);

        barometer.read();
        Serial.print("barome:\t");
        Serial.print(barometer.pres);
        Serial.print("\t");
        Serial.print(barometer.temp);
        Serial.print("\t");
        Serial.println(barometer.alti);

        Serial.println();

    } else {
        Serial.println("Подключенны не все датчики");
        setupSensors();
    }

    delay(1000);
}

void setupSensors() {
    accel_connection     = accel.begin();
    gyro_connection      = gyro.begin();
    compass_connection   = compass.begin();
    barometer_connection = barometer.begin();
}
