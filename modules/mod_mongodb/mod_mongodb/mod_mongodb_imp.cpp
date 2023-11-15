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
#include "mod_mongodb_imp.h" 

/*-----------------------------------------------------------------------------
 * MongoDB settings and workers
 *
 *---------------------------------------------------------------------------*/
MongoDBSettings mongodbSettings;
MongoDBWorkers  mongodbWorkers;

/*-----------------------------------------------------------------------------
 * MongoDB prototype functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * MongoDB database main functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * MongoDB timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_Timer1HzEvent(int)
{
	mongodbWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_mongodb_Timer1HzHandler()
{
	if (mongodbWorkers.timer1HzTrigger)
	{
		mongodbWorkers.timer1HzTrigger = false;
	}
}

/*-----------------------------------------------------------------------------
 * MongoDB message actions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * MongoDB message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{

};

/*-----------------------------------------------------------------------------
 * MongoDB Action register
 *
 *---------------------------------------------------------------------------*/
int mod_mongodb_RegisterMQTTActions()
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "registering MQTT actions");

	size_t 	index, 
			sizeofActionsTable = (sizeof(mod_mqtt_client_MQTTActionTable) /
								  sizeof(mod_mqtt_client_MQTTActionTable[0]));

	for (index = 0; index < sizeofActionsTable; index++)
	{
		lib_mqtt_AppendMQTTAction(mod_mqtt_client_MQTTActionTable[index]);
	}
 
	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MongoDB jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_mongodb_SettingsTable[] =
{
	{	MOD_MONGODB_SETTING_URI_STR,	mod_mongodb_SetMongoDBUri	}
};

/*-----------------------------------------------------------------------------
 * MongoDB settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_mongodb_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_mongodb_SettingsTable) /
									  sizeof(mod_mongodb_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_mongodb_SettingsTable[index].settingsName) &&
				(mod_mongodb_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_mongodb_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * MongoDB pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */
	mongodbSettings.mongoDBUri = MOD_MONGODB_INITIAL_URI_STR;

	/* Initialise the workers */
	mongodbWorkers.timer1HzId 		= 0;
	mongodbWorkers.timer1HzTrigger 	= false;

	mongodbWorkers.dbConnection.mongoClient = NULL;
	mongodbWorkers.dbConnection.mongoURI 	= NULL;
	mongodbWorkers.dbConnection.database 	= NULL;
	mongodbWorkers.dbConnection.collection 	= NULL;
}

/*-----------------------------------------------------------------------------
 * MongoDB post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_mongodb_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_MONGODB_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_mongodb_Timer1HzEvent,
										 &mongodbWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_mongodb_RegisterMQTTActions();

		mongoc_init();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}
/*-----------------------------------------------------------------------------
 * MongoDB module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_mongodb_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MongoDB mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_mongodb_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_mongodb_Timer1HzHandler();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/