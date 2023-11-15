/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_IMU_H
#define LIB_IMU_H

/*-----------------------------------------------------------------------------
 * Lib imu include imus
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"
extern "C" 
{
    #include <linux/i2c-dev.h>
    #include <linux/i2c.h>
    #include <i2c/smbus.h>
}

/*---------------------------------------------------------------------------*/
#include "../../application_interface/application_interface.h"
#include "../../lib_defines/lib_defines.h"

/*-----------------------------------------------------------------------------
 * Lib imu define values
 *
 *---------------------------------------------------------------------------*/
#define GYRO_RANGE 1
//	Gyroscope Range
//	0	+/- 250 degrees/second
//	1	+/- 500 degrees/second
//	2	+/- 1000 degrees/second
//	3	+/- 2000 degrees/second
//See the MPU6000 Register Map for more information

#define ACCEL_RANGE 2
//	Accelerometer Range
//	0	+/- 2g
//	1	+/- 4g
//	2	+/- 8g
//	3	+/- 16g
//See the MPU6000 Register Map for more information

//     Accelerometer
#define A_OFF_X 19402
#define A_OFF_Y -2692
#define A_OFF_Z -8625
//    Gyroscope
#define G_OFF_X -733
#define G_OFF_Y 433
#define G_OFF_Z -75

#define _POSIX_C_SOURCE 200809L //Used for calculating time

#define TAU 0.05 //Complementary filter percentage
#define RAD_T_DEG 57.29577951308 //Radians to degrees (180/PI)

//Select the appropriate settings
#if GYRO_RANGE == 1
	#define GYRO_SENS 65.5
	#define GYRO_CONFIG 0b00001000
#elif GYRO_RANGE == 2
	#define GYRO_SENS 32.8
	#define GYRO_CONFIG 0b00010000
#elif GYRO_RANGE == 3
	#define GYRO_SENS 16.4
	#define GYRO_CONFIG 0b00011000
#else //Otherwise, default to 0
	#define GYRO_SENS 131.0
	#define GYRO_CONFIG 0b00000000
#endif
#undef GYRO_RANGE


#if ACCEL_RANGE == 1
	#define ACCEL_SENS 8192.0
	#define ACCEL_CONFIG 0b00001000
#elif ACCEL_RANGE == 2
	#define ACCEL_SENS 4096.0
	#define ACCEL_CONFIG 0b00010000
#elif ACCEL_RANGE == 3
	#define ACCEL_SENS 2048.0
	#define ACCEL_CONFIG 0b00011000
#else //Otherwise, default to 0
	#define ACCEL_SENS 16384.0
	#define ACCEL_CONFIG 0b00000000
#endif
#undef ACCEL_RANGE

/*---------------------------------------------------------------------------*/
class MPU6050 {
	private:
		void _update();

		float _accel_angle[3];
		float _gyro_angle[3];
		float _angle[3];

		float ax, ay, az, gr, gp, gy;

		int MPU6050_addr;
		int f_dev;

		float dt;

		struct timespec start,end;

		bool _first_run = 1;
	public:
		MPU6050(int8_t addr);
		MPU6050(int8_t addr, bool run_update_thread);
		void getAccelRaw(float *x, float *y, float *z);
		void getGyroRaw(float *roll, float *pitch, float *yaw);
		void getAccel(float *x, float *y, float *z);
		void getGyro(float *roll, float *pitch, float *yaw);
		void getOffsets(float *ax_off, float *ay_off, float *az_off, float *gr_off, float *gp_off, float *gy_off);
		int getAngle(int axis, float *result);
		bool calc_yaw;
};

/*-----------------------------------------------------------------------------
 * Lib imu exposed functions
 *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
#endif /* LIB_IMU_H											                 */
/*---------------------------------------------------------------------------*/