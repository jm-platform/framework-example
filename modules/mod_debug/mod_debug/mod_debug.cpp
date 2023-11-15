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
 * Debug include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_debug.h"

/*---------------------------------------------------------------------------*/
#include "mod_debug_imp.h"

/*-----------------------------------------------------------------------------
 * Debug settings and workers
 *
 *---------------------------------------------------------------------------*/
extern DebugSettings debugSettings;
extern DebugWorkers  debugWorkers;

/*-----------------------------------------------------------------------------
 * Debug setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_debug_SetLoggingMode(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_DEBUG_SETTING_LOG_MODE_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_DEBUG_SETTING_LOG_MODE_STR_LENGTH_MAX))
	{
		try
		{
			debugSettings.logMode = std::stoul(settingsObjectData.settingsValue[0], nullptr, 16);

			debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
								"%s%s%s", 
						 		settingsObjectData.settingsName.c_str(),
								MOD_DEBUG_MESSAGE_EQUAL_STR, 
								settingsObjectData.settingsValue[0].c_str());
		}
		
		catch(const std::exception& e)
		{
			returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
		}
	}

	else
	{
		returnCode = RETURN_CODE_FAIL;
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_debug_SetLogDirectory(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if ((settingsObjectData.settingsCount == 1) &&
	    (settingsObjectData.settingsValue[0].length() <= MOD_DEBUG_SETTING_LOG_DIRECTORY_STR_LENGTH_MAX))
	{
		debugSettings.logDirectory = settingsObjectData.settingsValue[0];

		debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
							"%s%s%s", 
						 	settingsObjectData.settingsName.c_str(),
							MOD_DEBUG_MESSAGE_EQUAL_STR, 
							settingsObjectData.settingsValue[0].c_str());
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_debug_SetChannelEnabled(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if (settingsObjectData.settingsCount == 1)
	{
		if ((settingsObjectData.settingsValue[0] == LIB_DEFINE_STATEMENT_ON) ||
			(settingsObjectData.settingsValue[0] == LIB_DEFINE_STATEMENT_TRUE_STR))
		{
			debugSettings.channelEnabled = true;
		}

		else
		{
			debugSettings.channelEnabled = false;
		}

		debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
							"%s%s%s", 
						 	settingsObjectData.settingsName.c_str(),
							MOD_DEBUG_MESSAGE_EQUAL_STR, 
							settingsObjectData.settingsValue[0].c_str());
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_debug_SetChannelTimeout(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if (settingsObjectData.settingsCount == 1)
	{
		try
		{
			int timeout = std::stoul(settingsObjectData.settingsValue[0], nullptr, 16);

			if ((timeout >= MOD_DEBUG_SETTING_CHANNEL_TIMEOUT_MIN) && 
				(timeout <= MOD_DEBUG_SETTING_CHANNEL_TIMEOUT_MAX))
			{
				debugSettings.channelTimeout = timeout;
			}
		}
		
		catch(const std::exception& e)
		{
			returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
		}

		debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
							"%s%s%s", 
						 	settingsObjectData.settingsName.c_str(),
							MOD_DEBUG_MESSAGE_EQUAL_STR, 
							settingsObjectData.settingsValue[0].c_str());
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Debug module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_debug_ModuleInterface =
{

		/* application info */
        MOD_DEBUG_NAME,
		MOD_DEBUG_VERSION,
		MOD_DEBUG_BUILD,

		/* Exposed functions of the module */
		mod_debug_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_debug_SettingsHandler,				    /* Module PostInitialisationPtr */
		mod_debug_PostInitialisation,			    /* Module PostInitialisationPtr */
		mod_debug_DestroyData,					    /* Module DestroyDataPtr */
		mod_debug_MainloopFunction,				    /* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/