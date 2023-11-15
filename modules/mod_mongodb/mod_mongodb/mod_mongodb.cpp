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
 * MongoDB include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_mongodb.h"

/*---------------------------------------------------------------------------*/
#include "mod_mongodb_imp.h"

/*-----------------------------------------------------------------------------
 * MongoDB settings and workers
 *
 *---------------------------------------------------------------------------*/
extern MongoDBSettings mongodbSettings;
extern MongoDBWorkers  mongodbWorkers;

/*-----------------------------------------------------------------------------
 * MongoDB setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_mongodb_SetMongoDBUri(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MONGODB_SETTING_URI_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MONGODB_SETTING_URI_STR_LENGTH_MAX))
	{
		mongodbSettings.mongoDBUri = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}
	
	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MongoDB module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_mongodb_ModuleInterface =
{

		/* application info */
        MOD_MONGODB_NAME,
		MOD_MONGODB_VERSION,
		MOD_MONGODB_BUILD,

		/* Exposed functions of the module */
		mod_mongodb_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_mongodb_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_mongodb_PostInitialisation,				/* Module PostInitialisationPtr */
		mod_mongodb_DestroyData,					/* Module DestroyDataPtr */
		mod_mongodb_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/