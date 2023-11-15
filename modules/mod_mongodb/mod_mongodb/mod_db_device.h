/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_DB_DEVICE_H
#define MOD_DB_DEVICE_H

/*-----------------------------------------------------------------------------
 * mod MongoDB device include files
 *---------------------------------------------------------------------------*/
#include "mod_mongodb_defines.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB device define values
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * mod MongoDB device exposed functions
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_DeviceInit(DBDevice &dbDevice);
void mod_mongodb_DeviceClear(DBDevice &dbDevice);
int  mod_mongodb_DeviceReadFromJson(DBDevice &dbDevice, const char *jsonData);

/*---------------------------------------------------------------------------*/
void mod_mongodb_DeviceDataInit(DBDeviceData &dbDeviceData);
int  mod_mongodb_DeviceDataReadFromJson(DBDeviceData &dbDeviceData, const char *jsonData);

/*---------------------------------------------------------------------------*/
void mod_mongodb_DeviceActionsInit(DBDeviceActions &dbDeviceActions);
int  mod_mongodb_DeviceActionsReadFromJson(DBDeviceActions &dbDeviceActions, const char *jsonData);

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceProcessSubDeviceOnline(String &outputData,
											 const char *jsonData,							  
											 const String &dateTime,
											 const int &subDeviceId,
											 const bool online);
int mod_mongodb_DeviceProcessSubDeviceSensors(String &outputData,
											  const char *jsonData,							  
											  const String &dateTime,
											  const int &subDeviceId,
											  const int &heartbeat,
											  const int &oxygen,
											  const float &bodyTemperature,
											  const float &latitude,
											  const float &longitude,
											  const int &ambientNoise);
int mod_mongodb_DeviceProcessSubDeviceStatus(String &outputData,
											 const char *jsonData,							  
											 const String &dateTime,
											 const int &subDeviceId,
											 const String &proximityRange,
											 const String &physicalHealth,
											 const String &inCombat,
											 const String &emergency,
											 const String &support);
                                             
/*---------------------------------------------------------------------------*/
#endif /* MOD_DB_DEVICE_H											         */
/*---------------------------------------------------------------------------*/