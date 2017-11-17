#include <VB_L3G4200D.h>

VB_L3G4200D gyro; //создание объекта типа гироскоп L3G4200D

bool blinkState      = false;
bool gyro_connection = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial) {
        ; // Ожидаем когда появиться Serial Port через USB
    }
    setupGyro();
}

void loop() {
    // прочитать значения ускорений

    if (gyro_connection) {
        gyro.read();

        // отображение нормализованных измеренний (градусы в секунду)
        Serial.print("gyro:\t");
        Serial.print(gyro.x);
        Serial.print("\t");
        Serial.print(gyro.y);
        Serial.print("\t");
        Serial.println(gyro.z);

        // Моргать диодом изображая активность
        blinkState = !blinkState;
        digitalWrite(LED_BUILTIN, blinkState);
    }
    delay(500);
}

void setupGyro() {
    Serial.println("Инициализация I2C устройств...");

    // Ждем подключения пока не сможем подключиться
    while (!gyro_connection) {
        gyro_connection = gyro.begin();

        if (gyro_connection) {
            return;
        } else {
            Serial.println("L3G4200D соединение НЕ установлено");
            delay(500);
        }
    }
}
