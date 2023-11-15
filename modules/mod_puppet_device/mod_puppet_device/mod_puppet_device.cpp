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
 * Puppet Device include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_puppet_device.h"

/*---------------------------------------------------------------------------*/
#include "mod_puppet_device_imp.h"

/*-----------------------------------------------------------------------------
 * Puppet Device settings and workers
 *
 *---------------------------------------------------------------------------*/
extern PuppetDeviceSettings puppetDeviceSettings;
extern PuppetDeviceWorkers  puppetDeviceWorkers;

/*-----------------------------------------------------------------------------
 * Puppet Device setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_puppet_device_SetSensorInfoPath(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		puppetDeviceSettings.puppetDevicesPath = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_puppet_device_SetPuppetDeviceRole(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 2) &&
		(settingsObjectData.settingsValue[0].length() > 0) &&
		(settingsObjectData.settingsValue[1].length() > 0))
	{
		if ((settingsObjectData.settingsValue[0] == MOD_PUPPET_DEVICE_ROLE_TEST_STR) ||
			(settingsObjectData.settingsValue[0] == MOD_PUPPET_DEVICE_ROLE_PUPPET_STR))
		{
			puppetDeviceSettings.rolePuppet = settingsObjectData.settingsValue[0];
		}

		else
		{
			puppetDeviceSettings.rolePuppet = MOD_PUPPET_DEVICE_ROLE_MANAGER_STR;
		}

		puppetDeviceSettings.puppetAddress += std::stoi(settingsObjectData.settingsValue[1]);

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\",\"0x%llX\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str(),
									puppetDeviceSettings.puppetAddress);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_puppet_device_SetPrintNRFDetails(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		if (settingsObjectData.settingsValue[0] == LIB_DEFINE_STATEMENT_TRUE_STR)
		{
			puppetDeviceSettings.printNRFDetails = true;
		}

		else
		{
			puppetDeviceSettings.printNRFDetails = false;
		}

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_puppet_device_SetRFChannel(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		int rfChannel = std::stoi(settingsObjectData.settingsValue[0]);

		if (rfChannel <= MOD_PUPPET_DEVICE_RF_CHANNEL_MAX)
		{
			puppetDeviceSettings.rfChannel = rfChannel;

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
int	mod_puppet_device_SetRFPowerLevel(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		int rfPowerLevel = std::stoi(settingsObjectData.settingsValue[0]);

		if (rfPowerLevel <= MOD_PUPPET_DEVICE_RF_POWER_LEVEL_MAX)
		{
			puppetDeviceSettings.rfPowerLevel = (rf24_pa_dbm_e)rfPowerLevel;

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
int	mod_puppet_device_SetTimeout(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		int connectionTimeout = std::stoi(settingsObjectData.settingsValue[0]);

		if ((connectionTimeout >= MOD_PUPPET_DEVICE_CONNECTION_TIMOUT_MIN) &&
			(connectionTimeout <= MOD_PUPPET_DEVICE_CONNECTION_TIMOUT_MAX))
		{
			puppetDeviceSettings.connectionTimeout = connectionTimeout;

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
int	mod_puppet_device_SetEnableRF(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		if (settingsObjectData.settingsValue[0] == LIB_DEFINE_STATEMENT_FALSE_STR)
		{
			puppetDeviceSettings.rfEnabled = false;
		}

		else
		{
			puppetDeviceSettings.rfEnabled = true;
		}

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_puppet_device_SetNRFMode(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		if (settingsObjectData.settingsValue[0] == MOD_PUPPET_DEVICE_NRF_MODE_JMP_STR)
		{
			puppetDeviceSettings.nrfMode = nrfModeJMP;
		}

		else if (settingsObjectData.settingsValue[0] == MOD_PUPPET_DEVICE_NRF_MODE_DWMN_STR)
		{
			puppetDeviceSettings.nrfMode = nrfModeDWMN;
		}

		else if (settingsObjectData.settingsValue[0] == MOD_PUPPET_DEVICE_NRF_MODE_NORMAL_STR)
		{
			puppetDeviceSettings.nrfMode = nrfModeNormal;
		}

		else
		{
			puppetDeviceSettings.nrfMode = nrfModeNone;
		}

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Puppet Device module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_puppet_device_ModuleInterface =
{

		/* application info */
        MOD_PUPPET_DEVICE_NAME,
		MOD_PUPPET_DEVICE_VERSION,
		MOD_PUPPET_DEVICE_BUILD,

		/* Exposed functions of the module */
		mod_puppet_device_PreInitialisation,		/* Module PreInitialisationPtr */
		mod_puppet_device_SettingsHandler,			/* Module PostInitialisationPtr */
		mod_puppet_device_PostInitialisation,		/* Module PostInitialisationPtr */
		mod_puppet_device_DestroyData,				/* Module DestroyDataPtr */
		mod_puppet_device_MainloopFunction,			/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/