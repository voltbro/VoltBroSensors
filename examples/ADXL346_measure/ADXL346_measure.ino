#include <VB_ADXL346.h>

VB_ADXL346 accel; //создание объекта типа акселерометр ADXL346

bool blinkState       = false;
bool accel_connection = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial) {
        ; // Ожидаем когда появиться Serial Port через USB
    }
    setupAccel();
}

void loop() {
    // прочитать значения ускорений
    if (accel_connection) {
        //Нормализованные измерения (м/с^2)
        accel.read();

        Serial.print("accel:\t");
        Serial.print(accel.x);
        Serial.print("\t");
        Serial.print(accel.y);
        Serial.print("\t");
        Serial.println(accel.z);

        // Моргать диодом изображая активность
        blinkState = !blinkState;
        digitalWrite(LED_BUILTIN, blinkState);

        delay(500);
    }
}

void setupAccel() {
    Serial.println("Инициализация I2C устройств...");

    while (!accel_connection) {
        accel_connection = accel.begin();

        if (accel_connection) {
            return;
        } else {
            Serial.println("ADXL346 соединение НЕ установлено");
            delay(500);
        }
    }
}
