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
#include "app_skeleton_imp.h" 

/*-----------------------------------------------------------------------------
 * Skeleton settings and workers
 *
 *---------------------------------------------------------------------------*/
SkeletonSettings skeletonSettings;
SkeletonWorkers  skeletonWorkers;

/*-----------------------------------------------------------------------------
 * Skeleton prototype functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Skeleton timer functions
 *---------------------------------------------------------------------------*/
void app_skeleton_Timer1HzEvent(int)
{
	skeletonWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void app_skeleton_Timer1HzHandler()
{
	if (skeletonWorkers.timer1HzTrigger)
	{
		skeletonWorkers.timer1HzTrigger = false;

		//lib_mqtt_PublishMessage(1, "test test");
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	app_skeleton_ActionSkeletonTest(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
								MOD_DEBUG_MESSAGE_EQUAL_STR, 
								mqttMessage.messageData.c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{
	{	LIB_MQTT_TOPIC_RESOURCE_SKELETON_TEST	,	app_skeleton_ActionSkeletonTest		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int app_skeleton_RegisterMQTTActions()
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
 * Skeleton jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject app_skeleton_SettingsTable[] =
{
	{	APP_SKELETON_SETTING_TEST_STR,	app_skeleton_SetSettingTest	}
};

/*-----------------------------------------------------------------------------
 * Skeleton settings handler
 *
 *---------------------------------------------------------------------------*/
int app_skeleton_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(app_skeleton_SettingsTable) /
									  sizeof(app_skeleton_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == app_skeleton_SettingsTable[index].settingsName) &&
				(app_skeleton_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(app_skeleton_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Skeleton pre initialisation
 *
 *---------------------------------------------------------------------------*/
void app_skeleton_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */

	/* Initialise the workers */
	skeletonWorkers.timer1HzId 		= 0;
	skeletonWorkers.timer1HzTrigger = false;

}

/*-----------------------------------------------------------------------------
 * Skeleton post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int app_skeleton_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_APP_SKELETON_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)app_skeleton_Timer1HzEvent,
										 &skeletonWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		app_skeleton_RegisterMQTTActions();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}
/*-----------------------------------------------------------------------------
 * Skeleton module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int app_skeleton_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Skeleton mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int app_skeleton_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        app_skeleton_Timer1HzHandler();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/