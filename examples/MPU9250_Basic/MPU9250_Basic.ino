
#include <VB_MPU9250.h>

VB_MPU9250 imu;

void setup() 
{
  Serial.begin(115200);

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to it's default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if (imu.begin() != MPU_OK)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }
 
}

void loop() 
{
  // dataReady() checks to see if new accel/gyro data
  // is available. It will return a boolean true or false
  // (New magnetometer data cannot be checked, as the library
  //  runs that sensor in single-conversion mode.)
  imu.read();
  printIMUData();
  
}

void printIMUData(void)
{  
  Serial.print("Accel raw: ax = " + String(imu.ax_raw) + ", ay = " +
              String(imu.ay_raw) + ", az = " + String(imu.az_raw) + " ");
  Serial.println("Gyro raw: gx = " + String(imu.gx_raw) + ", gy = " +
              String(imu.gy_raw) + ", gz = " + String(imu.gz_raw) + " ");
  Serial.println("Mag raw: mx = " + String(imu.mx_raw) + ", my = " +
              String(imu.my_raw) + ", mz = " + String(imu.mz_raw) + " ");
  //SerialPort.println("Time: " + String(imu.time) + " ms");
  Serial.println();
}

