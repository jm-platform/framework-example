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
 * Notifications include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_notifications.h"

/*---------------------------------------------------------------------------*/
#include "mod_notifications_imp.h"

/*-----------------------------------------------------------------------------
 * Notifications settings and workers
 *
 *---------------------------------------------------------------------------*/
extern NotificationsSettings notificationsSettings;
extern NotificationsWorkers  notificationsWorkers;

/*-----------------------------------------------------------------------------
 * Notifications setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_notifications_SetMailFolderPath(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_NOTIFICATIONS_SETTING_MAIL_FOLDER_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_NOTIFICATIONS_SETTING_MAIL_FOLDER_STR_LENGTH_MAX))
	{
		notificationsSettings.mailFolderPath = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Notifications module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_notifications_ModuleInterface =
{

		/* application info */
        MOD_NOTIFICATIONS_NAME,
		MOD_NOTIFICATIONS_VERSION,
		MOD_NOTIFICATIONS_BUILD,

		/* Exposed functions of the module */
		mod_notifications_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_notifications_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_notifications_PostInitialisation,			/* Module PostInitialisationPtr */
		mod_notifications_DestroyData,					/* Module DestroyDataPtr */
		mod_notifications_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/