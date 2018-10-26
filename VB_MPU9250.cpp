/*
++VVM created by V.Medinskiy 14/03/18 

This library implements motion processing functions of Invensense's MPU-9250.
It is based on their Emedded MotionDriver 6.12 library.
	https://www.invensense.com/developers/software-download


*/


#include <VB_MPU9250.h>

#include <Wire.h>
#include <Arduino.h>


extern "C" {
#include "inv_mpu.h"
}

VB_MPU9250::VB_MPU9250()
{
	_mSense = 6.6693f; // Constant - 4912 / 32760
	_aSense = 0.0f;   // Updated after accel FSR is set
	_gSense = 0.0f;   // Updated after gyro FSR is set
}

mpu_error_t VB_MPU9250::begin(void)
{
	mpu_error_t result;
    struct int_param_s int_param;

	Wire.begin();

	result = mpu_init(&int_param);

	if (result)
		return result;

	mpu_set_bypass(1); // Place all slaves (including compass) on primary bus

	// Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
	mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

// Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  setAccelFSR(16); // Set accel to +/-16g
  // Note: the MPU-9250's magnetometer FSR is set at
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  mpu_set_lpf(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  mpu_set_sample_rate(10); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  mpu_set_compass_sample_rate(10); // Set mag rate to 10Hz

	return result;
}

void VB_MPU9250::read_raw()
{
	if ( dataReady() )
  {
    // Call update() to update the imu objects sensor data.
    // You can specify which sensors to update by combining
    // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
    // UPDATE_TEMPERATURE.
    // (The update function defaults to accel, gyro, compass,
    //  so you don't have to specify these values.)
    update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    }
}

void VB_MPU9250::read()
{
	read_raw();
	ax = (float) ax_raw / (float) _aSense;
	ay = (float) ay_raw / (float) _aSense;
	az = (float) az_raw / (float) _aSense;
	gx = (float) gx_raw / (float) _gSense;
	gy = (float) gy_raw / (float) _gSense;
	gz = (float) gz_raw / (float) _gSense;
	mx = (float) mx_raw / (float) _mSense;
	my = (float) my_raw / (float) _mSense;
	mz = (float) mz_raw / (float) _mSense;

}

mpu_error_t VB_MPU9250::setGyroFSR(unsigned short fsr)
{
	mpu_error_t err;
	err = mpu_set_gyro_fsr(fsr);
	if (err == MPU_OK)
	{
		_gSense = getGyroSens();
	}
	return err;
}

mpu_error_t VB_MPU9250::setAccelFSR(unsigned char fsr)
{
	mpu_error_t err;
	err = mpu_set_accel_fsr(fsr);
	if (err == MPU_OK)
	{
		_aSense = getAccelSens();
	}
	return err;
}

float VB_MPU9250::getGyroSens(void)
{
	float sens;
	if (mpu_get_gyro_sens(&sens) == MPU_OK)
	{
		return sens;
	}
	return 0;
}

unsigned short VB_MPU9250::getAccelSens(void)
{
	unsigned short sens;
	if (mpu_get_accel_sens(&sens) == MPU_OK)
	{
		return sens;
	}
	return 0;
}

bool VB_MPU9250::dataReady()
{
	unsigned char intStatusReg;

	if (mpu_read_reg(0x3A, &intStatusReg) == MPU_OK)
	{
		return (intStatusReg & (1<<0));
	}
	return false;
}

mpu_error_t VB_MPU9250::update(unsigned char sensors)
{
	mpu_error_t aErr = MPU_OK;
	mpu_error_t gErr = MPU_OK;
	mpu_error_t mErr = MPU_OK;
	mpu_error_t tErr = MPU_OK;

	if (sensors & UPDATE_ACCEL)
		aErr = updateAccel();
	if (sensors & UPDATE_GYRO)
		gErr = updateGyro();
	if (sensors & UPDATE_COMPASS)
		mErr = updateCompass();
	if (sensors & UPDATE_TEMP)
		tErr = updateTemperature();

	return aErr | gErr | mErr | tErr;
}


int VB_MPU9250::updateAccel(void)
{
	short data[3];

	if (mpu_get_accel_reg(data, &time))
	{
		return MPU_ERROR;
	}
	ax_raw = data[0];
	ay_raw = data[1];
	az_raw = data[2];
	return MPU_OK;
}

int VB_MPU9250::updateGyro(void)
{
	short data[3];

	if (mpu_get_gyro_reg(data, &time))
	{
		return MPU_ERROR;
	}
	gx_raw = data[0];
	gy_raw = data[1];
	gz_raw = data[2];
	return MPU_OK;
}

int VB_MPU9250::updateCompass(void)
{
	short data[3];

	if (mpu_get_compass_reg(data, &time))
	{
		return MPU_ERROR;
	}
	mx_raw = data[0];
	my_raw = data[1];
	mz_raw = data[2];
	return MPU_OK;
}

mpu_error_t VB_MPU9250::updateTemperature(void)
{
	return mpu_get_temperature(&temperature, &time);
}
