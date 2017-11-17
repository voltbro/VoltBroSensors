#include <VB_L3G4200D.h>

VB_L3G4200D gyro; //создание объекта типа гироскоп L3G4200D

bool gyro_connection = false;

// Таймеры
unsigned long timer = 0;
float timeStep      = 0.01;

// Pitch, Roll and Yaw values, углы тангаж, крен, рысканье
float pitch = 0;
float roll  = 0;
float yaw   = 0;

void setup() {
    Serial.begin(115200);

    gyro.default_scale = L3G4200D_FULL_SCALE_250;
    gyro_connection    = gyro.begin();
}

void loop() {
    timer = millis();

    // прочитать значения ускорений

    if (gyro_connection) {
        gyro.read();
        // Расчет углов
        pitch = pitch + gyro.y * timeStep;
        roll  = roll + gyro.x * timeStep;
        yaw   = yaw + gyro.z * timeStep;

        Serial.print(" Pitch/Тангаж = ");
        Serial.print(pitch);
        Serial.print(" Roll/Крен = ");
        Serial.print(roll);
        Serial.print(" Yaw/Рысканье = ");
        Serial.println(yaw);
    }
    // Ждем
    delay((timeStep * 1000) - (millis() - timer));
}
