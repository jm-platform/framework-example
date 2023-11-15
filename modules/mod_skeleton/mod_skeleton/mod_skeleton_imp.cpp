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
#include "mod_skeleton_imp.h" 

/*-----------------------------------------------------------------------------
 * Skeleton settings and workers
 *
 *---------------------------------------------------------------------------*/
SkeletModSettings skeletModSettings;
SkeletModWorkers  skeletModWorkers;

/*-----------------------------------------------------------------------------
 * Skeleton prototype functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Skeleton timer functions
 *---------------------------------------------------------------------------*/
void mod_skeleton_Timer1HzEvent(int)
{
	skeletModWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_skeleton_Timer1HzHandler()
{
	if (skeletModWorkers.timer1HzTrigger)
	{
		skeletModWorkers.timer1HzTrigger = false;

		//lib_mqtt_PublishMessage(1, "test test");
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_skeleton_ActionSkeletonTest(MQTTMessage &mqttMessage)
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
	{	LIB_MQTT_TOPIC_RESOURCE_SKELETON_TEST	,	mod_skeleton_ActionSkeletonTest		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_skeleton_RegisterMQTTActions()
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
const SettingsObjectTableObject mod_skeleton_SettingsTable[] =
{
	{	MOD_SKELETON_SETTING_TEST_STR,	mod_skeleton_SetSettingTest	}
};

/*-----------------------------------------------------------------------------
 * Skeleton settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_skeleton_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_skeleton_SettingsTable) /
									  sizeof(mod_skeleton_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_skeleton_SettingsTable[index].settingsName) &&
				(mod_skeleton_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_skeleton_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Skeleton pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_skeleton_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */

	/* Initialise the workers */
	skeletModWorkers.timer1HzId 		= 0;
	skeletModWorkers.timer1HzTrigger = false;

}

/*-----------------------------------------------------------------------------
 * Skeleton post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_skeleton_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_SKELETON_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_skeleton_Timer1HzEvent,
										 &skeletModWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_skeleton_RegisterMQTTActions();

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
int mod_skeleton_DestroyData()
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
int mod_skeleton_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_skeleton_Timer1HzHandler();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/