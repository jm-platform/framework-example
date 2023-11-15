/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MDB_DEVICE_CORE_H
#define MOD_MDB_DEVICE_CORE_H

/*-----------------------------------------------------------------------------
 * mod MongoDB device core include files
 *---------------------------------------------------------------------------*/
#include "mod_mongodb_defines.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB device core define values
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * mod MongoDB device core type definitions
 *
 *---------------------------------------------------------------------------*/
typedef struct DeviceValue
{
	String deviceId;
	String type;
	String name;
	String icon;
	String date;
	String value;
	String notation;
	String unit;

} DeviceValue;

/*-----------------------------------------------------------------------------
 * mod MongoDB device core exposed functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_core_ReadDeviceById(DBDevice &dbDevice, const String &deviceId);

/*---------------------------------------------------------------------------*/
int mod_mdb_device_core_UpdateOnlineById(const bool &online, const String &deviceId);
int mod_mdb_device_core_AddDeviceValue(const DeviceValue &deviceValue);
int mod_mdb_device_core_AddNotificationById(const String &date,
											const String &level,
											const String &message,
											const String &deviceId);

/*---------------------------------------------------------------------------*/
#endif /* MOD_MDB_DEVICE_CORE_H											     */
/*---------------------------------------------------------------------------*/
