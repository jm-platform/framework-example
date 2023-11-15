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
#include "mod_notifications_imp.h" 

/*-----------------------------------------------------------------------------
 * Notifications settings and workers
 *
 *---------------------------------------------------------------------------*/
NotificationsSettings notificationsSettings;
NotificationsWorkers  notificationsWorkers;

/*-----------------------------------------------------------------------------
 * Notifications prototype functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Notifications message functions
 *
 *---------------------------------------------------------------------------*/
int mod_notifications_NotifyOnline(const String &deviceId)
{
	String dateTime = lib_time_GetDateTimeStringISO();

	int returnCode = mod_mdb_device_core_AddNotificationById(dateTime, 
													 MOD_NOTIFICATIONS_LEVEL_NOTICE, 
													 MOD_NOTIFICATIONS_MESSAGE_ONLINE, 
													 deviceId);

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_notifications_NotifyOffline(const String &deviceId)
{
	String dateTime = lib_time_GetDateTimeStringISO();

	int returnCode = mod_mdb_device_core_AddNotificationById(dateTime, 
													 MOD_NOTIFICATIONS_LEVEL_ALERT, 
													 MOD_NOTIFICATIONS_MESSAGE_OFFLINE, 
													 deviceId);

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_notifications_NotifyAlarmState(const String &deviceId, const String &alarm)
{
	String dateTime 	= lib_time_GetDateTimeStringISO();
	String alarmState 	= String(MOD_NOTIFICATIONS_MESSAGE_ALARM) + alarm;

	int returnCode = mod_mdb_device_core_AddNotificationById(dateTime, 
													 MOD_NOTIFICATIONS_LEVEL_NOTICE, 
													 alarmState, 
													 deviceId);

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_notifications_NotifyGeneral(const String &deviceId, 
									const String &level,
									const String &message)
{
	String dateTime = lib_time_GetDateTimeStringISO();

	int returnCode = mod_mdb_device_core_AddNotificationById(dateTime, 
													 level, 
													 message, 
													 deviceId);

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Notifications email functions
 *
 *---------------------------------------------------------------------------*/
void mod_notifications_SendEmail(const int &amount, 
								 const String &mailFolderPath)
{
	char mailStr[255];

	for (int index = 0; index < amount; index++)
	{
		if (notificationsWorkers.notificationEmails[index].length() > 0)
		{
			sprintf(mailStr, "ssmtp -v %s < %s > /dev/null 2>&1\n", 
					notificationsWorkers.notificationEmails[index].c_str(), 
					mailFolderPath.c_str());

			int status __attribute__((unused));
			status = system(mailStr);
		}
	}
}

/*---------------------------------------------------------------------------*/
int mod_notifications_SendAlarmEmails(const String &deviceId)
{
	int returnCode = RETURN_CODE_OK;

	DBDeviceData dbDeviceData;

	mod_mongodb_DeviceDataInit(dbDeviceData);

	returnCode = mod_mdb_device_io_ReadDeviceDataById(dbDeviceData, deviceId);

	for (int index = 0; index < dbDeviceData.emailsAmount; index++)
	{
		notificationsWorkers.notificationEmails[index] = dbDeviceData.notificationEmails[index];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "sending alarm email to: \"%s\"",
				notificationsWorkers.notificationEmails[index].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	std::thread serialReplyHandler(mod_notifications_SendEmail,
								   dbDeviceData.emailsAmount, 
								   notificationsSettings.mailFolderPath);

	serialReplyHandler.detach();

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Notifications timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_notifications_Timer1HzEvent(int)
{
	notificationsWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_notifications_Timer1HzHandler()
{
	if (notificationsWorkers.timer1HzTrigger)
	{
		notificationsWorkers.timer1HzTrigger = false;
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_notifications_ActionNotificationsTest(MQTTMessage &mqttMessage)
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
	//{	LIB_MQTT_TOPIC_RESOURCE_NOTIFICATIONS_TEST	,	mod_notifications_ActionNotificationsTest		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_notifications_RegisterMQTTActions()
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
 * Notifications jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_notifications_SettingsTable[] =
{
	{	MOD_NOTIFICATIONS_SETTING_MAIL_FOLDER_STR,	mod_notifications_SetMailFolderPath	}
};

/*-----------------------------------------------------------------------------
 * Notifications settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_notifications_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_notifications_SettingsTable) /
									  sizeof(mod_notifications_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_notifications_SettingsTable[index].settingsName) &&
				(mod_notifications_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_notifications_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Notifications pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_notifications_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */
	notificationsSettings.mailFolderPath = MOD_NOTIFICATIONS_INITIAL_MAIL_FOLDER_STR;

	/* Initialise the workers */
	notificationsWorkers.timer1HzId 		= 0;
	notificationsWorkers.timer1HzTrigger = false;

}

/*-----------------------------------------------------------------------------
 * Notifications post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_notifications_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_NOTIFICATIONS_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_notifications_Timer1HzEvent,
										 &notificationsWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_notifications_RegisterMQTTActions();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}
/*-----------------------------------------------------------------------------
 * Notifications module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_notifications_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Notifications mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_notifications_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_notifications_Timer1HzHandler();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/