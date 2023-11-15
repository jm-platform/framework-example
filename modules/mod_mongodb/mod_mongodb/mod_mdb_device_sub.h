/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MDB_DEVICE_SUB_H
#define MOD_MDB_DEVICE_SUB_H

/*-----------------------------------------------------------------------------
 * mod MongoDB device sub include files
 *---------------------------------------------------------------------------*/
#include "mod_mongodb_defines.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB device sub define values
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * mod MongoDB device sub type definitions
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * mod MongoDB device sub exposed functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_sub_UpdateSubDeviceOnline(const String deviceId,
									  			const String &dateTime,
									  			const int &subDeviceId,
									  			const bool online);
int mod_mdb_device_sub_UpdateSubDeviceSensorData(const String deviceId,
										  			const String &dateTime,
										  			const int &subDeviceId,
										  			const int	&heartbeat,
										  			const int	&oxygen,
										  			const float &bodyTemperature,
										  			const float &latitude,
										  			const float &longitude,
										  			const int	&ambientNoise);
int mod_mdb_device_sub_UpdateSubDeviceStatus(const String deviceId,
									  			const String &dateTime,
									 	 		const int &subDeviceId,
									  			const String &proximityRange,
									  			const String &physicalHealth,
									  			const String &inCombat,
									  			const String &emergency,
									  			const String &support);

/*---------------------------------------------------------------------------*/
#endif /* MOD_MDB_DEVICE_SUB_H											     */
/*---------------------------------------------------------------------------*/
