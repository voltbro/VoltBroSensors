#include <VB_ADXL345.h>

VB_ADXL345 accel; //создание объекта типа акселерометр ADXL345

bool blinkState = false;
bool adxlconnection;

void setup() {

  Serial.begin(115200);
  adxlconnection = accel.begin();
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

  // прочитать значения ускорений

  if (adxlconnection) {
    //Нормализованные измерения (м/с^2)
    accel.read();

     // Calculate Pitch & Roll
    int pitch = -(atan2(accel.x, sqrt(accel.y*accel.y + accel.z*accel.z))*180.0)/M_PI;
    int roll  = (atan2(accel.y, accel.z)*180.0)/M_PI;

    Serial.print(" Pitch = ");
    Serial.print(pitch);
    Serial.print(" Roll = ");
    Serial.print(roll);
    Serial.println();

    // Моргать диодом изображая активность
    blinkState = !blinkState;
    digitalWrite(LED_BUILTIN, blinkState);

  }
  delay(100);
}
