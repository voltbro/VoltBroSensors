#include <VB_ADXL345.h>

VB_ADXL345 accel; //создание объекта типа акселерометр ADXL345

bool blinkState = false;
bool adxlconnection = false;

//int16_t ax, ay, az;
float ax, ay, az;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
    // remove waiting if device work without USB link
  }
  setupADXL();

}

void loop() {

  // прочитать значения ускорений

  if (adxlconnection) {
    //accel.getAccelerationRaw(&ax, &ay, &az);
    //Нормализованные измерения (м/с^2)
    accel.getAccelerationNorm(&ax, &ay, &az);

    // отображение измеренных значений
    Serial.print("accel:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.println(az);

    // Моргать диодом изображая активность
    blinkState = !blinkState;
    digitalWrite(LED_BUILTIN, blinkState);

  }
  delay(500);

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
