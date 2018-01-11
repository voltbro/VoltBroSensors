#include <VB_ADXL346.h>

VB_ADXL346 accel; //создание объекта типа акселерометр ADXL346

bool accel_connection = false;

void setup() {
    Serial.begin(115200);
    accel_connection = accel.begin();
}

void loop() {
    // прочитать значения ускорений

    if (accel_connection) {
        //Нормализованные измерения (м/с^2)
        accel.read();

        // Calculate Pitch & Roll
        int pitch = -(atan2(accel.x, sqrt(accel.y * accel.y + accel.z * accel.z)) * 180.0) / M_PI;
        int roll  = (atan2(accel.y, accel.z) * 180.0) / M_PI;

        Serial.print(" Pitch = ");
        Serial.print(pitch);
        Serial.print(" Roll = ");
        Serial.print(roll);
        Serial.println();
    }
    delay(100);
}
