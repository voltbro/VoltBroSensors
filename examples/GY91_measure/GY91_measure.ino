#include <VB_MPU9250.h>
#include <VB_BMP280.h>

VB_BMP280 barometer; //создание объекта типа акселерометр ADXL345
VB_MPU9250 imu;

bool imu_connection;
bool barometer_connection;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Ожидаем когда появиться Serial Port через USB
    }
    setupSensors();
}

void loop() {
    if ((imu_connection && barometer_connection)) {
        imu.read();
        Serial.print("accel:\t");
        Serial.print(imu.ax);
        Serial.print("\t");
        Serial.print(imu.ay);
        Serial.print("\t");
        Serial.println(imu.az);

        Serial.print("gyros:\t");
        Serial.print(imu.gx);
        Serial.print("\t");
        Serial.print(imu.gy);
        Serial.print("\t");
        Serial.println(imu.gz);

        Serial.print("compass:\t");
        Serial.print(imu.mx);
        Serial.print("\t");
        Serial.print(imu.my);
        Serial.print("\t");
        Serial.println(imu.mz);

        barometer.read();
        Serial.print("barometer:\t");
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
    imu_connection   = (imu.begin() == MPU_OK);
    barometer.start_altitude = 100;
    barometer_connection = barometer.begin();
}
