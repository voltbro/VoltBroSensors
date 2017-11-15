#include <VB_ADXL345.h>

VB_ADXL345 accel; //создание объекта типа акселерометр ADXL345

bool blinkState = false;
bool adxlconnection;

float ax, ay, az;

void setup() {

  Serial.begin(115200);
  adxlconnection = accel.begin();
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

  // прочитать значения ускорений

  if (adxlconnection) {
    //Нормализованные измерения (м/с^2)
    accel.getAccelerationNorm(&ax, &ay, &az);

     // Calculate Pitch & Roll
    int pitch = -(atan2(ax, sqrt(ay*ay + az*az))*180.0)/M_PI;
    int roll  = (atan2(ay, az)*180.0)/M_PI;

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
