#include <VB_HMC5883L.h>

VB_HMC5883L compass; //создание объекта для компаса HMC5883L

bool compass_connection = false;

int16_t mx, my, mz;
float mx_norm, my_norm, mz_norm;

void setup() {
    // Инициализация ком порта
    Serial.begin(115200);

    while (!Serial) {
        ; // Ожидаем когда появиться Serial Port через USB
    }
    setupCompas();
}

void loop() {
    if (compass_connection) {
        compass.read();

        // Расчет направления магнетометра/компаса
        float heading = atan2(compass.y, compass.x);

        // После расчета направления необходимо добавить угол магнитного склонения 'Declination Angle', этот угол зависит от места проведения измерения и является поправкой для компаса.
        // Свой угол можно найти здесь : http://www.magnetic-declination.com/
        // Мой (г. Москва, г. Троицк) : 10° 53',  10.883 градуса, или 0.18994 радиан

        float declinationAngle = 0.18994;
        heading += declinationAngle;

        // Исправим знак - попробовал, мне не понравилось
        //if(heading < 0) heading += 2*PI;

        // Исправим значение угла если больше 360° - так ни разу не было, можно не добавлять
        //if(heading > 2*PI) heading -= 2*PI;
        // Переводим радианы в градусы.
        float headingDegrees = heading * 180 / M_PI;

        Serial.print("Raw:\t");
        Serial.print(compass.x_raw);
        Serial.print("   ");
        Serial.print(compass.y_raw);
        Serial.print("   ");
        Serial.print(compass.z_raw);
        Serial.print("   \tNorm:\t");

        Serial.print(compass.x);
        Serial.print("   ");
        Serial.print(compass.y);
        Serial.print("   ");
        Serial.print(compass.z);

        Serial.print("   \tHeading:\t");
        Serial.print(heading);
        Serial.print(" Radians   \t");
        Serial.print(headingDegrees);
        Serial.println(" Degrees   \t");
    }

    // По-хорошему надо делать задержку 66ms, чтобы цикл выполнялся
    // с частотой 15Hz (по умолчанию для HMC5883L).
    // Но так как ком порт работает медленно (104ms на частоте 9600),
    // то задержку можно не делать.

    // delay(66);
}

void setupCompas() {
    Serial.println("Инициализация I2C устройств...");

    while (!compass_connection) {
        compass_connection = compass.begin();

        if (compass_connection) {
            return;
        } else {
            Serial.println("HMC5883L соединение НЕ установлено");
            delay(500);
        }
    }
}
