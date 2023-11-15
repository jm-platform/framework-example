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
 * GPRS include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_gprs.h"

/*---------------------------------------------------------------------------*/
#include "mod_gprs_imp.h"

/*-----------------------------------------------------------------------------
 * GPRS settings and workers
 *
 *---------------------------------------------------------------------------*/
extern GPRSSettings gprsSettings;
extern GPRSWorkers  gprsWorkers;

/*-----------------------------------------------------------------------------
 * GPRS setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_gprs_SetSerialPort(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_GPRS_SETTING_SERIAL_PORT_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_GPRS_SETTING_SERIAL_PORT_STR_LENGTH_MAX))
	{
		gprsSettings.serialPort = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_gprs_SetSerialBaudrate(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		int baudrate = stoi(settingsObjectData.settingsValue[0]);

		if (baudrate <= MOD_GPRS_SETTING_SERIAL_BAUDRATE_MAX)
		{
			gprsSettings.serialBaudrate = baudrate;

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
int	mod_gprs_SetLocationUpdateInterval(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		int interval = stoi(settingsObjectData.settingsValue[0]);

		if ((interval >= MOD_GPRS_SETTING_LOCATION_UPDATE_INTERVAL_MIN) &&
			(interval <= MOD_GPRS_SETTING_LOCATION_UPDATE_INTERVAL_MAX))
		{
			gprsSettings.locationUpdateInterval = interval;

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
int	mod_gprs_SetStartUpDelay(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		int startupDelay = stoi(settingsObjectData.settingsValue[0]);

		if (startupDelay <= MOD_GPRS_SETTING_STARTUP_DELAY_MAX)
		{
			gprsSettings.startupDelay = startupDelay;

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
 * GPRS module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_gprs_ModuleInterface =
{

		/* application info */
        MOD_GPRS_NAME,
		MOD_GPRS_VERSION,
		MOD_GPRS_BUILD,

		/* Exposed functions of the module */
		mod_gprs_PreInitialisation,			    	/* Module PreInitialisationPtr */
		mod_gprs_SettingsHandler,					/* Module PostInitialisationPtr */
		mod_gprs_PostInitialisation,				/* Module PostInitialisationPtr */
		mod_gprs_DestroyData,						/* Module DestroyDataPtr */
		mod_gprs_MainloopFunction,					/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/