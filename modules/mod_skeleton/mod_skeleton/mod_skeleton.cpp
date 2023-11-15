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
 * Skeleton include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_skeleton.h"

/*---------------------------------------------------------------------------*/
#include "mod_skeleton_imp.h"

/*-----------------------------------------------------------------------------
 * Skeleton settings and workers
 *
 *---------------------------------------------------------------------------*/
extern SkeletModSettings skeletModSettings;
extern SkeletModWorkers  skeletModWorkers;

/*-----------------------------------------------------------------------------
 * Skeleton setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_skeleton_SetSettingTest(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "%s%s%s", settingsObjectData.settingsName.c_str(),
								MOD_DEBUG_MESSAGE_EQUAL_STR, 
								settingsObjectData.settingsValue[0].c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Skeleton module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_skeleton_ModuleInterface =
{

		/* application info */
        MOD_SKELETON_NAME,
		MOD_SKELETON_VERSION,
		MOD_SKELETON_BUILD,

		/* Exposed functions of the module */
		mod_skeleton_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_skeleton_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_skeleton_PostInitialisation,			/* Module PostInitialisationPtr */
		mod_skeleton_DestroyData,					/* Module DestroyDataPtr */
		mod_skeleton_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/