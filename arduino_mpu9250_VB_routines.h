/*
++VVM created by V.Medinskiy 14/03/18 

This library implements motion processing functions of Invensense's MPU-9250.
It is based on their Emedded MotionDriver 6.12 library.
	https://www.invensense.com/developers/software-download


*/

#ifndef _ARDUINO_MPU9250_VB_ROUTINES_H_
#define _ARDUINO_MPU9250_VB_ROUTINES_H_

#define MPL_LOG_UNKNOWN		(0)
#define MPL_LOG_DEFAULT		(1)
#define MPL_LOG_VERBOSE		(2)
#define MPL_LOG_DEBUG		(3)
#define MPL_LOG_INFO		(4)
#define MPL_LOG_WARN		(5)
#define MPL_LOG_ERROR		(6)
#define MPL_LOG_SILENT		(8)

typedef enum {
    PACKET_DATA_ACCEL = 0,
    PACKET_DATA_GYRO,
    PACKET_DATA_COMPASS,
    PACKET_DATA_QUAT,
    PACKET_DATA_EULER,
    PACKET_DATA_ROT,
    PACKET_DATA_HEADING,
    PACKET_DATA_LINEAR_ACCEL,
    NUM_DATA_PACKETS
} eMPL_packet_e;




#if defined(__cplusplus)
extern "C" {
#endif
#include <stdarg.h>

int arduino_get_clock_ms(unsigned long *count);
int arduino_delay_ms(unsigned long num_ms);

void logString(char * string);

void eMPL_send_quat(long *quat);
void eMPL_send_data(unsigned char type, long *data);

int arduino_i2c_write(unsigned char slave_addr, unsigned char reg_addr,
                       unsigned char length, unsigned char * data);
int arduino_i2c_read(unsigned char slave_addr, unsigned char reg_addr,
                       unsigned char length, unsigned char * data);

#if defined(__cplusplus)
}
#endif


#endif // _ARDUINO_MPU9250_VB_ROUTINES_H_
