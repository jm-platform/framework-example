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
#include "app_skeleton.h"

/*---------------------------------------------------------------------------*/
#include "app_skeleton_imp.h"

/*-----------------------------------------------------------------------------
 * Skeleton settings and workers
 *
 *---------------------------------------------------------------------------*/
extern SkeletonSettings skeletonSettings;
extern SkeletonWorkers  skeletonWorkers;

/*-----------------------------------------------------------------------------
 * Skeleton setter function
 *
 *---------------------------------------------------------------------------*/
int	app_skeleton_SetSettingTest(SettingsObjectData &SettingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "%s%s%s", SettingsObjectData.settingsName.c_str(),
								MOD_DEBUG_MESSAGE_EQUAL_STR, 
								SettingsObjectData.settingsValue[0].c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Skeleton module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface app_skeleton_ModuleInterface =
{

		/* application info */
        APP_SKELETON_NAME,
		APP_SKELETON_VERSION,
		APP_SKELETON_BUILD,

		/* Exposed functions of the module */
		app_skeleton_PreInitialisation,			    /* Module PreInitialisationPtr */
		app_skeleton_SettingsHandler,				/* Module PostInitialisationPtr */
		app_skeleton_PostInitialisation,			/* Module PostInitialisationPtr */
		app_skeleton_DestroyData,					/* Module DestroyDataPtr */
		app_skeleton_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/