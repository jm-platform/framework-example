/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MDB_DEVICE_IO_H
#define MOD_MDB_DEVICE_IO_H

/*-----------------------------------------------------------------------------
 * mod MongoDB device io include files
 *---------------------------------------------------------------------------*/
#include "mod_mongodb_defines.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB device io define values
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * mod MongoDB device io type definitions
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * mod MongoDB device io exposed functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_io_ReadDeviceDataById(DBDeviceData &dbDeviceData, const String &deviceId);
int mod_mdb_device_io_ReadDeviceActionsById(DBDeviceActions &dbDeviceActions, const String &deviceId);

/*---------------------------------------------------------------------------*/
int mod_mdb_device_io_UpdateDeviceInputById(const bool &digitalInput, const String &deviceId);
int mod_mdb_device_io_UpdateDeviceLocationById(const float &latitude,
										 	   const float &longitude,
										 	   const int &accuracy,
										 	   const String &dateTime,
										 	   const String &deviceId);	
int mod_mdb_device_io_UpdateDeviceAlarmById(const String &AlarmState, const String &deviceId);

/*---------------------------------------------------------------------------*/
#endif /* MOD_MDB_DEVICE_IO_H											     */
/*---------------------------------------------------------------------------*/
