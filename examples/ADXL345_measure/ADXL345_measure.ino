#include <VB_ADXL345.h>

VB_ADXL345 accel; //создание объекта типа акселерометр ADXL345

bool blinkState = false;
bool adxlconnection = false;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    ; // Ожидаем когда появиться Serial Port через USB
  }
  setupADXL();

}

void loop() {

  // прочитать значения ускорений
  if (adxlconnection) {

    //Нормализованные измерения (м/с^2)
    accel.read();

    Serial.print("accel:\t");
    Serial.print(accel.x); Serial.print("\t");
    Serial.print(accel.y); Serial.print("\t");
    Serial.println(accel.z);

    // Моргать диодом изображая активность
    blinkState = !blinkState;
    digitalWrite(LED_BUILTIN, blinkState);

    delay(500);
  }


}

void setupADXL() {

  Serial.println("Инициализация I2C устройств...");

  while ( !adxlconnection) {

    Serial.println("Проверяем соединение с устройством...");
    adxlconnection = accel.begin();

    if (adxlconnection) {
      return;
    } else {
      Serial.println("ADXL345 соединение НЕ установлено");
      delay(500);
    }

  }

}
