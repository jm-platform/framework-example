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
 * IO include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_io.h"

/*---------------------------------------------------------------------------*/
#include "mod_io_imp.h"

/*-----------------------------------------------------------------------------
 * IO settings and workers
 *
 *---------------------------------------------------------------------------*/
extern IOSettings ioSettings;
extern IOWorkers  ioWorkers;

/*-----------------------------------------------------------------------------
 * IO setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_io_SetSystemType(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_IO_SETTING_SYSTEM_TYPE_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_IO_SETTING_SYSTEM_TYPE_STR_LENGTH_MAX))
	{
		ioSettings.systemType = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s%s", settingsObjectData.settingsName.c_str(),
								  MOD_DEBUG_MESSAGE_EQUAL_STR, 
								  settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * IO module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_io_ModuleInterface =
{

		/* application info */
        MOD_IO_NAME,
		MOD_IO_VERSION,
		MOD_IO_BUILD,

		/* Exposed functions of the module */
		mod_io_PreInitialisation,			    	/* Module PreInitialisationPtr */
		mod_io_SettingsHandler,						/* Module PostInitialisationPtr */
		mod_io_PostInitialisation,					/* Module PostInitialisationPtr */
		mod_io_DestroyData,							/* Module DestroyDataPtr */
		mod_io_MainloopFunction,					/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/