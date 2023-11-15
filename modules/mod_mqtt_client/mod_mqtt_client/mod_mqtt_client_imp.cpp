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
 * MQTT Client include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_mqtt_client_imp.h" 

/*-----------------------------------------------------------------------------
 * MQTT Client settings and workers
 *
 *---------------------------------------------------------------------------*/
MQTTClientSettings mqttClientSettings;
MQTTClientWorkers  mqttClientWorkers;

/*-----------------------------------------------------------------------------
 * MQTT Client prototype functions
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * MQTT Client general functions
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_PublishMessage(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_FAIL;

	if (mqttClientWorkers.mqttClient->connected)
	{
		if (mqttMessage.messageDest == LIB_MQTT_TOPIC_SERVER_STR)
		{
			mqttMessage.messageDest = mqttClientSettings.serverID;
		}

		String topic;

		if (!mqttMessage.messageSubTopic.empty())
		{
			topic = lib_mqtt_CreateTopic(mqttClientSettings.clientID,
										 mqttMessage.messageDest,
										 mqttMessage.messageTopicID,
										 mqttMessage.messageSubTopic);
		}

		else
		{
			topic = lib_mqtt_CreateTopic(mqttClientSettings.clientID,
										 mqttMessage.messageDest,
										 mqttMessage.messageTopicID);
		}

		if (topic != LIB_MQTT_TOPIC_INVALID_STR)
		{
			mqttClientWorkers.mqttClient->publish(NULL, 
												  topic.c_str(), 
												  mqttMessage.messageData.length(), 
												  mqttMessage.messageData.c_str(),
												  LIB_MQTT_QOS_AT_MOST_ONCE);
			
			debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, 
								"published to: %s", topic.c_str());

			returnCode = RETURN_CODE_OK;
		}

		else
		{
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, "MQTT publish invalid");
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client callback functions
 *
 *---------------------------------------------------------------------------*/
void mod_mqtt_client_RegisterConnection()
{
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "MQTT connection successful");

	mqttClientWorkers.reconnectTimer	= 0;

	size_t topicLength = mqttClientSettings.serverID.length() +
						 sizeof(LIB_MQTT_TOPIC_SEPERATOR_STR) +
						 mqttClientSettings.clientID.length() +
						 sizeof(LIB_MQTT_TOPIC_SEPERATOR_STR) +
						 sizeof(LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_CONNECT);

	char topic[topicLength];
	snprintf(topic, topicLength, "%s%s%s%s%d", mqttClientSettings.serverID.c_str(),
											   LIB_MQTT_TOPIC_SEPERATOR_STR,
											   mqttClientSettings.clientID.c_str(),
											   LIB_MQTT_TOPIC_SEPERATOR_STR,
											   LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_CONNECT);

	mqttClientWorkers.mqttClient->publish(NULL, 
										  topic, 
										  mqttClientSettings.clientID.length(), 
										  mqttClientSettings.clientID.c_str(),
										  LIB_MQTT_QOS_EXACTLY_ONCE);
}

/*---------------------------------------------------------------------------*/
int mod_mqtt_client_SubscribeGeneralTopic()
{
	int returnCode = RETURN_CODE_OK;

	size_t topicLength = mqttClientSettings.clientID.length() +
						 sizeof(LIB_MQTT_TOPIC_WILDCARD_STR);

	char commonTopic[topicLength];
	snprintf(commonTopic, topicLength, "%s%s", mqttClientSettings.clientID.c_str(),
								  			   LIB_MQTT_TOPIC_WILDCARD_STR);

	returnCode = mqttClientWorkers.mqttClient->subscribe(NULL, commonTopic);

	if (returnCode == RETURN_CODE_OK)
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "subscribed to: \"%s\"", commonTopic);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	else
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "subscribed to: \"%s\" failed", commonTopic);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);

		returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		size_t connectionstopicLength = sizeof(LIB_MQTT_TOPIC_CONNECTIONS_STR) +
						 	 			sizeof(LIB_MQTT_TOPIC_WILDCARD_STR);

		char connectionstopic[connectionstopicLength];
		snprintf(connectionstopic, connectionstopicLength, "%s%s", 
				 LIB_MQTT_TOPIC_CONNECTIONS_STR,
				 LIB_MQTT_TOPIC_WILDCARD_STR);
		
		returnCode = mqttClientWorkers.mqttClient->subscribe(NULL, connectionstopic);

		if (returnCode == RETURN_CODE_OK)
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "subscribed to: \"%s\"", connectionstopic);
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}

		else
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "subscribed to: \"%s\" failed", connectionstopic);
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);

			returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
		}		
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_mqtt_client_DataListAppend(MQTTMessage &mqttMessage)
{
	mqttClientWorkers.mqttMessageList.push_back(mqttMessage);
}

/*---------------------------------------------------------------------------*/
void mod_mqtt_client_ReconnectClient()
{
	mqttClientWorkers.mqttClient->reconnect_async();

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, "MQTT connection failed");
}

/*---------------------------------------------------------------------------*/
void mod_mqtt_client_AppendMessageAction(const MQTTMessageAction &MessageAction)
{
	mqttClientWorkers.mqttMessageActionList.push_back(MessageAction);

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "appending MQTT action \"%d\"", MessageAction.topicID);
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
}

/*-----------------------------------------------------------------------------
 * MQTT Client mainloop functions
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_DataHandler()
{
	int returnCode = RETURN_CODE_OK;

	if (!mqttClientWorkers.mqttMessageList.empty())
	{
		bool processed = false;

		MQTTMessage mqttMessage = mqttClientWorkers.mqttMessageList.front();

		mqttClientWorkers.mqttMessageList.pop_front();

		if (!mqttClientWorkers.mqttMessageActionList.empty())
		{
			for (MQTTMessageActionList::const_iterator mqttMessageActionIt = 
				mqttClientWorkers.mqttMessageActionList.begin();
				mqttMessageActionIt != mqttClientWorkers.mqttMessageActionList.end(); mqttMessageActionIt++)
			{
				if ((mqttMessageActionIt->topicID == mqttMessage.messageTopicID) &&
					(mqttMessageActionIt->mqttAction != NULL))
				{
					(mqttMessageActionIt->mqttAction)(mqttMessage);

					processed = true;
				}
			}	
		}

		if (!processed)
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "MQTT message \"%s\" not processed", mqttMessage.messageTopic.c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
		}	
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Skeleton timer functions
 *---------------------------------------------------------------------------*/
void mod_mqtt_client_Timer1HzEvent(int)
{
	mqttClientWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_mqtt_client_Timer1HzHandler()
{
	if (mqttClientWorkers.timer1HzTrigger)
	{
		mqttClientWorkers.timer1HzTrigger = false;

		if (!mqttClientWorkers.mqttClient->connected)
		{
			if (++mqttClientWorkers.reconnectTimer >= mqttClientSettings.reconnectDelay)
			{
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, "MQTT connection failed");
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, "reconnecting to MQTT");

				mqttClientWorkers.reconnectTimer = 0;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------n------------------------------------------*/
int	mod_mqtt_client_ActionRegisterConection(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	if (mqttMessage.messageData == LIB_MQTT_MESSAGE_DATA_ALL_STR)
	{
		debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
						"received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
						MOD_DEBUG_MESSAGE_EQUAL_STR, 
						mqttMessage.messageData.c_str());

		mod_mqtt_client_RegisterConnection();
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_ActionGeneral(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
						"received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
						MOD_DEBUG_MESSAGE_EQUAL_STR, 
						mqttMessage.messageData.c_str());

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_ActionDeviceInfo(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;
	
	debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
						"received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
						MOD_DEBUG_MESSAGE_EQUAL_STR, 
						mqttMessage.messageData.c_str());

	String info = "error: info type unknown";
	
	if (mqttMessage.messageData == LIB_MQTT_DEVICE_INFO_STORAGE_STR)
	{
		info = lib_system_GetStorageCapacityString();
		lib_mqtt_publish_PublishNotification(LIB_MQTT_NOTIFICATION_INFO_STR, info);
	}

	else if (mqttMessage.messageData == LIB_MQTT_DEVICE_INFO_SYSTEM_STR)
	{
		info = lib_system_GetLinuxInfoString();
		lib_mqtt_publish_PublishNotification(LIB_MQTT_NOTIFICATION_INFO_STR, info);
	}

	else if (mqttMessage.messageData == LIB_MQTT_DEVICE_INFO_PLATFORM_STR)
	{
		info = lib_system_GePlatformInfoString();
		lib_mqtt_publish_PublishNotification(LIB_MQTT_NOTIFICATION_INFO_STR, info);
	}

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, info.c_str());

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_ActionMQTTVersion(MQTTMessage &mqttMessage)
{
	debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
						"received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
						MOD_DEBUG_MESSAGE_EQUAL_STR, 
						mqttMessage.messageData.c_str());

	int returnCode = RETURN_CODE_OK;

	int major 	 = 0;
	int minor	 = 0;
	int revision = 0;

	mosqpp::lib_version(&major, &minor, &revision);

	debug_masked_printf(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
						"Mosquittopp version: %d.%d.%d",
						major,
						minor,
						revision);

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{
	{	LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_REGISTER,	mod_mqtt_client_ActionRegisterConection	},
	{	LIB_MQTT_TOPIC_RESOURCE_GENERAL				,	mod_mqtt_client_ActionGeneral			},
	{	LIB_MQTT_TOPIC_RESOURCE_DEVICE_INFO			,	mod_mqtt_client_ActionDeviceInfo		},
	{	LIB_MQTT_TOPIC_RESOURCE_MQTT_VERSION		,	mod_mqtt_client_ActionMQTTVersion		},
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_RegisterMQTTActions()
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
 * MQTT Client jump table is used for inifile settings
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_mqtt_client_SettingsTable[] =
{
	{	MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR		,	mod_mqtt_client_SetClientId			},
	{	MOD_MQTT_CLIENT_SETTING_SERVER_ID_STR		,	mod_mqtt_client_SetServerId			},
	{	MOD_MQTT_CLIENT_SETTING_SERVER_ADDRESS_STR	,	mod_mqtt_client_SetServerAdress		},
	{	MOD_MQTT_CLIENT_SETTING_SERVER_PORT_STR		,	mod_mqtt_client_SetServerPort		},
	{	MOD_MQTT_CLIENT_SETTING_SERVER_LOGIN_STR	,	mod_mqtt_client_SetServerUsername	},
	{	MOD_MQTT_CLIENT_SETTING_SERVER_PSWRD_STR	,	mod_mqtt_client_SetServerPassword	},
	{	MOD_MQTT_CLIENT_SETTING_RECONNECT_DELAY_STR	,	mod_mqtt_client_SetReconnectDelay	},
	{	MOD_MQTT_CLIENT_SETTING_LOG_MODE_STR		,	mod_mqtt_client_SetLogMode			}
};

/*-----------------------------------------------------------------------------
 * MQTT Client settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_mqtt_client_SettingsTable) /
									  sizeof(mod_mqtt_client_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_mqtt_client_SettingsTable[index].settingsName) &&
				(mod_mqtt_client_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_mqtt_client_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_mqtt_client_PreInitialisation()
{
	/* Initialise the settings */
	mqttClientSettings.clientID 		= MOD_MQTT_CLIENT_INITIAL_CLIENT_ID_STR;
	mqttClientSettings.serverID 		= MOD_MQTT_CLIENT_INITIAL_SERVER_ID_STR;
	mqttClientSettings.serverAddress	= MOD_MQTT_CLIENT_INITIAL_SERVER_ADDRESS_STR;
	mqttClientSettings.serverPort		= MOD_MQTT_CLIENT_INITIAL_SERVER_PORT;
	mqttClientSettings.serverUsername	= MOD_MQTT_CLIENT_INITIAL_SERVER_LOGIN_STR;
	mqttClientSettings.serverPassword	= MOD_MQTT_CLIENT_INITIAL_SERVER_PSWRD_STR;
	mqttClientSettings.logMode			= MOD_MQTT_CLIENT_INITIAL_LOG_MODE_STR;

	/* Initialise the workers */
	mqttClientWorkers.timer1HzId 		= 0;
	mqttClientWorkers.timer1HzTrigger 	= false;
	mqttClientWorkers.reconnectTimer	= 0;

	mqttClientWorkers.mqttMessageList.clear();
	mqttClientWorkers.mqttMessageActionList.clear();
	
	lib_mqtt_RegisterCallbacks(&mod_mqtt_client_PublishMessage,
							   &mod_mqtt_client_AppendMessageAction);

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
}

/*-----------------------------------------------------------------------------
 * MQTT Client post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_MQTT_CLIENT_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_mqtt_client_Timer1HzEvent,
										 &mqttClientWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mosqpp::lib_init();

		/* Initialize MQTT connection */
		mqttClientWorkers.mqttClient = new MQTTClient;		
		mqttClientWorkers.mqttClient->reinitialise(mqttClientSettings.clientID.c_str(), false);

		mqttClientWorkers.mqttClient->username_pw_set(mqttClientSettings.serverUsername.c_str(),
													  mqttClientSettings.serverPassword.c_str());

		/* Create last will */
		mqttClientWorkers.mqttClient->logMode = mqttClientSettings.logMode;
		size_t topicLength = mqttClientSettings.serverID.length() +
							 sizeof(LIB_MQTT_TOPIC_SEPERATOR_STR) +
							 mqttClientSettings.clientID.length() +
							 sizeof(LIB_MQTT_TOPIC_SEPERATOR_STR) +
							 sizeof(LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCONNECT);

		char topic[topicLength];
		snprintf(topic, topicLength, "%s%s%s%s%d", mqttClientSettings.serverID.c_str(),
												   LIB_MQTT_TOPIC_SEPERATOR_STR,
												   mqttClientSettings.clientID.c_str(),
												   LIB_MQTT_TOPIC_SEPERATOR_STR,
												   LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCONNECT);

		mqttClientWorkers.mqttClient->will_set(topic, 
											   mqttClientSettings.clientID.length(), 
											   mqttClientSettings.clientID.c_str(),
											   LIB_MQTT_QOS_EXACTLY_ONCE);
		
		/* Connect to MQTT broker */
		mqttClientWorkers.mqttClient->connect_async(mqttClientSettings.serverAddress.c_str(),
												    mqttClientSettings.serverPort);

		/* Register callback functions */
		mqttClientWorkers.mqttClient->registerCallbackFunctions(&mod_mqtt_client_RegisterConnection,
																&mod_mqtt_client_SubscribeGeneralTopic,
																&mod_mqtt_client_DataListAppend,
																&mod_mqtt_client_ReconnectClient);


		mod_mqtt_client_RegisterMQTTActions();

		/* Create new thread for mosquitto loop */
		mqttClientWorkers.mqttClient->loop_start();

		return returnCode; 
	}

	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}
/*-----------------------------------------------------------------------------
 * MQTT Client module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */
	mosqpp::lib_cleanup();

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_mqtt_client_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

		mod_mqtt_client_Timer1HzHandler();

		if (mqttClientWorkers.mqttClient->connected)
		{
			returnCode = mod_mqtt_client_DataHandler();
		}

		return returnCode;
	}

	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/