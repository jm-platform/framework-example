/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Gps include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_gps.h"

/*---------------------------------------------------------------------------*/
#include "mod_gps_imp.h"

/*-----------------------------------------------------------------------------
 * Gps settings and workers
 *
 *---------------------------------------------------------------------------*/
extern GpsSettings gpsSettings;
extern GpsWorkers  gpsWorkers;

/*-----------------------------------------------------------------------------
 * Gps setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_gps_SetSerialPort(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_GPS_SETTING_SERIAL_PORT_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_GPS_SETTING_SERIAL_PORT_STR_LENGTH_MAX))
	{
		gpsSettings.serialPort = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_gps_SetSerialBaudrate(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		int baudrate = stoi(settingsObjectData.settingsValue[0]);

		if (baudrate <= MOD_GPS_SETTING_SERIAL_BAUDRATE_MAX)
		{
			gpsSettings.serialBaudrate = baudrate;

			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
										MOD_DEBUG_MESSAGE_EQUAL_STR, 
										settingsObjectData.settingsValue[0].c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_gps_SetLocationUpdateInterval(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		int interval = stoi(settingsObjectData.settingsValue[0]);

		if ((interval >= MOD_GPS_SETTING_LOCATION_UPDATE_INTERVAL_MIN) &&
			(interval <= MOD_GPS_SETTING_LOCATION_UPDATE_INTERVAL_MAX))
		{
			gpsSettings.locationUpdateInterval = interval;

			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
										MOD_DEBUG_MESSAGE_EQUAL_STR, 
										settingsObjectData.settingsValue[0].c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Gps module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_gps_ModuleInterface =
{
		/* application info */
        MOD_GPS_NAME,
		MOD_GPS_VERSION,
		MOD_GPS_BUILD,

		/* Exposed functions of the module */
		mod_gps_PreInitialisation,			    	/* Module PreInitialisationPtr */
		mod_gps_SettingsHandler,					/* Module PostInitialisationPtr */
		mod_gps_PostInitialisation,					/* Module PostInitialisationPtr */
		mod_gps_DestroyData,						/* Module DestroyDataPtr */
		mod_gps_MainloopFunction,					/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */
};

/*---------------------------------------------------------------------------*/