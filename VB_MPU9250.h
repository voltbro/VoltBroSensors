/*
++VVM created by V.Medinskiy 14/03/18

This library implements motion processing functions of Invensense's MPU-9250.
It is based on their Emedded MotionDriver 6.12 library.
	https://www.invensense.com/developers/software-download


*/

#ifndef _VB_MPU9250_H_
#define _VB_MPU9250_H_

#define MPU9250
#define ARDUINO_VB
#define AK8963_SECONDARY
#define COMPASS_ENABLED


extern "C" {
#include "inv_mpu.h"
}

typedef int mpu_error_t;
#define MPU_OK 0
#define MPU_ERROR 0x20

// Define's passed to update(), to request a specific sensor (or multiple):
#define UPDATE_ACCEL   (1<<1)
#define UPDATE_GYRO    (1<<2)
#define UPDATE_COMPASS (1<<3)
#define UPDATE_TEMP    (1<<4)

class VB_MPU9250
{
public:
	int ax_raw, ay_raw, az_raw;
	int gx_raw, gy_raw, gz_raw;
	int mx_raw, my_raw, mz_raw;

	float ax, ay, az;
	float gx, gy, gz;
	float mx, my, mz;
	//long qw, qx, qy, qz;
	long temperature;
	unsigned long time;
	//float pitch, roll, yaw;
	//float heading;

	VB_MPU9250();
	void read_raw(void);
	void read(void);

	// begin(void) -- Verifies communication with the MPU-9250 and the AK8963,
	// and initializes them to the default state:
	// All sensors enabled
	// Gyro FSR: +/- 2000 dps
	// Accel FSR: +/- 2g
	// LPF: 42 Hz
	// FIFO: 50 Hz, disabled
	// Output: MPU_OK (0) on success, otherwise error
	mpu_error_t begin(void);

	// setGyroFSR(unsigned short) -- Sets the full-scale range of the gyroscope
	// Input: Gyro DPS - 250, 500, 1000, or 2000
	// Output: MPU_OK (0) on success, otherwise error
	mpu_error_t setGyroFSR(unsigned short fsr);
	// getGyroSens -- Returns current gyroscope sensitivity. The FSR divided by
	// the resolution of the sensor (signed 16-bit).
	// Output: Currently set gyroscope sensitivity (e.g. 131, 65.5, 32.8, 16.4)
	float getGyroSens(void);

	// setAccelFSR(unsigned short) -- Sets the FSR of the accelerometer
	//
	// Input: Accel g range - 2, 4, 8, or 16
	// Output: MPU_OK (0) on success, otherwise error
	mpu_error_t setAccelFSR(unsigned char fsr);
	// getAccelSens -- Returns current accelerometer sensitivity. The FSR
	// divided by the resolution of the sensor (signed 16-bit).
	// Output: Currently set accel sensitivity (e.g. 16384, 8192, 4096, 2048)
	unsigned short getAccelSens(void);

	mpu_error_t update(unsigned char sensors =
	                   UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);

	// updateAccel, updateGyro, updateCompass, and updateTemperature are
	// called by the update() public method. They read from their respective
	// sensor and update the class variable (e.g. ax, ay, az)
	// Output: MPU_OK (0) on success, otherwise error
	mpu_error_t updateAccel(void);
	mpu_error_t updateGyro(void);
	mpu_error_t updateCompass(void);
	mpu_error_t updateTemperature(void);

private:
	unsigned short _aSense;
	float _gSense, _mSense;
	bool dataReady(void);
};

#endif  /* #ifndef _VB_MPU9250_H_ */
