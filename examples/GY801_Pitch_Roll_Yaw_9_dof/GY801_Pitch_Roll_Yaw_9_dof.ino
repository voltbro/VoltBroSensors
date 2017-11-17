#include <VB_ADXL345.h>
#include <VB_HMC5883L.h>
#include <VB_L3G4200D.h>

VB_ADXL345 accel; //создание объекта типа акселерометр ADXL345
VB_L3G4200D gyro; //создание объекта типа гироскоп L3G4200D
VB_HMC5883L compass;

#define sampleFreq 512.0f // sample frequency in Hz
#define betaDef 0.1f      // 2 * proportional gain

bool gyro_connection;
bool accel_connection;
bool compass_connection;

// Таймеры
unsigned long timer = 0;
float timeStep      = 0.01;

uint32_t LastSec    = 0;
float deltaT        = 0.3f;
volatile float beta = betaDef;                             // 2 * proportional gain (Kp)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f; // quaternion of sensor frame relative to auxiliary frame

float angles[3];

void setup() {
    gyro.default_scale = L3G4200D_FULL_SCALE_250;

    Serial.begin(115200);
    while (!Serial) {
        ; // Ожидаем когда появиться Serial Port через USB
    }

    accel_connection = accel.begin();
    Serial.println(accel_connection ? "ADXL345 соединение установлено" : "ADXL345 соединение не установлено");

    gyro_connection = gyro.begin();
    Serial.println(gyro_connection ? "L3G4200D соединение установлено" : "L3G4200D соединение не установлено");

    compass_connection = compass.begin();
    Serial.println(compass_connection ? "HMC5883L соединение установлено" : "HMC5883L соединение не установлено");
}

void loop() {
    uint8_t i;
    // put your main code here, to run repeatedly:
    if (accel_connection && gyro_connection && compass_connection) {
        IMU_GetYawPitchRoll(angles, millis());

        Serial.print(" Pitch/Тангаж = ");
        Serial.print(angles[2]);
        Serial.print(" Roll/Крен = ");
        Serial.print(angles[1]);
        Serial.print(" Yaw/Рысканье = ");
        Serial.println(angles[0]);
    }
}

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3,
        q3q3;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0   = 2.0f * q0;
        _2q1   = 2.0f * q1;
        _2q2   = 2.0f * q2;
        _2q3   = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0   = q0 * q0;
        q0q1   = q0 * q1;
        q0q2   = q0 * q2;
        q0q3   = q0 * q3;
        q1q1   = q1 * q1;
        q1q2   = q1 * q2;
        q1q3   = q1 * q3;
        q2q2   = q2 * q2;
        q2q3   = q2 * q3;
        q3q3   = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx   = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy   = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) +
             _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) +
             (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) +
             (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) +
             (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) +
             _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    q0 += qDot1 * deltaT;
    q1 += qDot2 * deltaT;
    q2 += qDot3 * deltaT;
    q3 += qDot4 * deltaT;

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0        = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1        = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2        = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3        = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    q0 += qDot1 * deltaT;
    q1 += qDot2 * deltaT;
    q2 += qDot3 * deltaT;
    q3 += qDot4 * deltaT;
    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
    float halfx = 0.5f * x;
    float y     = x;
    long i      = *(long *)&y;
    i           = 0x5f3759df - (i >> 1);
    y           = *(float *)&i;
    y           = y * (1.5f - (halfx * y * y));
    return y;
    /*unsigned int i = 0x5F1F1412 - (*(unsigned int*)&x >> 1);
    float tmp = *(float*)&i;
    return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
      */
}

void IMU_GetQuater(void) {
    float MotionVal[9];

    gyro.read();
    MotionVal[0] = gyro.x;
    MotionVal[1] = gyro.y;
    MotionVal[2] = gyro.z;

    accel.read();
    MotionVal[3] = accel.x_raw;
    MotionVal[4] = accel.y_raw;
    MotionVal[5] = accel.z_raw;

    compass.read();
    MotionVal[6] = compass.x_raw;
    MotionVal[7] = compass.y_raw;
    MotionVal[8] = compass.z_raw;

    MadgwickAHRSupdate((float)MotionVal[0] * 0.0175, (float)MotionVal[1] * 0.0175, (float)MotionVal[2] * 0.0175, (float)MotionVal[3], (float)MotionVal[4],
                       (float)MotionVal[5], (float)MotionVal[6], (float)MotionVal[7], (float)MotionVal[8]);
    //MadgwickAHRSupdateIMU((float)MotionVal[0] * 0.0175, (float)MotionVal[1] * 0.0175, (float)MotionVal[2] * 0.0175,
    //(float)MotionVal[3], (float)MotionVal[4], (float)MotionVal[5]);
}

void IMU_GetYawPitchRoll(float *Angles, uint32_t S) {
    deltaT  = ((float)(S - LastSec)) / 1000.0;
    LastSec = S;

    IMU_GetQuater();
    Angles[1] = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;                                 // pitch
    Angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll
    Angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;
}
