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
 * GPRS include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_gprs_imp.h" 

#include "../../mod_io/mod_io.h" 

/*-----------------------------------------------------------------------------
 * GPRS settings and workers
 *
 *---------------------------------------------------------------------------*/
GPRSSettings gprsSettings;
GPRSWorkers  gprsWorkers;

/*---------------------------------------------------------------------------*/
static pthread_mutex_t *gprsMutex;

/*-----------------------------------------------------------------------------
 * GPRS prototype functions
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
bool mod_gprs_CheckSerialReply();
String mod_gprs_GetSerialReply();
void mod_gprs_SetSerialReply(const String &reply);

/*-----------------------------------------------------------------------------
 * GPRS publish functions
 *
 *---------------------------------------------------------------------------*/
void mod_gprs_PublishLocation(String latitude, String longitude, String accuracy)
{
	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "%s %s %s", latitude.c_str(), longitude.c_str(), accuracy.c_str());
	lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_LOCATION, buffer);
}

/*-----------------------------------------------------------------------------
 * GPRS general functions
 *
 *---------------------------------------------------------------------------*/
int	mod_gprs_ProcessMessageLocation(int &argumentCount, String arguments[])
{
	int returnCode = RETURN_CODE_OK;

	if ((argumentCount == 3) && (!arguments[1].empty()))
	{
		int dataCount = 0;
		String argumentData[4];

		mod_gprs_TokenizeSerialMessage(arguments[1], 
									   dataCount, 
									   argumentData, 
									   MOD_GPRS_MESSAGE_DATA_SEPERATOR);

		
		if (argumentData[0] == MOD_GPRS_MESSAGE_DATA_CODE_SUCCES)
		{
			if (dataCount == 4)
			{
				if ((!argumentData[1].empty()) && 
					(!argumentData[2].empty()) &&
					(!argumentData[3].empty()))
				{
					gprsWorkers.locationData.latitude 	= argumentData[2];
					gprsWorkers.locationData.longitude 	= argumentData[1];
					gprsWorkers.locationData.accuracy 	= argumentData[3];

					char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
					sprintf(buffer, "location received: la:%s lo:%s acc:%sM",
							gprsWorkers.locationData.latitude.c_str(),
							gprsWorkers.locationData.longitude.c_str(),
							gprsWorkers.locationData.accuracy.c_str());
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

					mod_gprs_PublishLocation(gprsWorkers.locationData.latitude, 
											 gprsWorkers.locationData.longitude,
											 gprsWorkers.locationData.accuracy);
				}
			}
		}

		else
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "ERROR: gprs connection failed, code %s", argumentData[0].c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_gprs_ProcessSerialMessage()
{
	int returnCode = RETURN_CODE_OK;

	String message;
	message.clear();

	if (!mod_gprs_CheckSerialReply())
	{
		String messageArguments[MOD_GPRS_MESSAGE_ARGUMENTS_MAX];
		int argumentCount = 0;

		message = mod_gprs_GetSerialReply();

		mod_gprs_TokenizeSerialMessage(message, 
									   argumentCount, 
									   messageArguments, 
									   MOD_GPRS_MESSAGE_ARGUMENTS_SEPERATOR);

		if (messageArguments[0] == MOD_GPRS_COMMAND_STATUS_OK_STR)
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "gprs message \"%s\" received", message.c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}

		else if (messageArguments[0] == MOD_GPRS_COMMAND_STATUS_ERROR_STR)
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "ERROR: gprs message \"%s\" received", message.c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
		}

		else if (messageArguments[0] == MOD_GPRS_COMMAND_LOCATION_STR)
		{
			returnCode = mod_gprs_ProcessMessageLocation(argumentCount, messageArguments);
		}

		else
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "ERROR: gprs message \"%s\" unknown", message.c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_gprs_ReadFromSerialConnection()
{
	int returnCode = RETURN_CODE_OK;

	struct timespec sleepTimeRequest, sleepTimeRemain;
	sleepTimeRequest.tv_sec 	= MOD_IO_INPUT_LOOP_SLEEP_INTERVAL / 1000000000;
	sleepTimeRequest.tv_nsec 	= MOD_IO_INPUT_LOOP_SLEEP_INTERVAL % 1000000000;

	int messageLength = 0;

	String message, messageBuffer;
	messageBuffer.clear();
	message.clear();

	while (gprsWorkers.getReply)
	{
		returnCode = lib_serial_ReadFromConnection(gprsWorkers.serialConnection, 
												   messageLength, 
												   messageBuffer);

		if ((messageLength > 0) && (returnCode == RETURN_CODE_OK))
		{
			message.append(messageBuffer);
			messageBuffer.clear();

			messageLength = 0;
		}

		if ((message.find(MOD_GPRS_MESSAGE_END_OK_STR) != String::npos) ||
			(message.find(MOD_GPRS_MESSAGE_END_ERROR_STR) != String::npos))
		{
			mod_gprs_ConvertSerialMessage(message);

			mod_gprs_SetSerialReply(message);

			message.clear();
		}

		nanosleep(&sleepTimeRequest, &sleepTimeRemain);
	}
}

/*---------------------------------------------------------------------------*/
void mod_gprs_InitializeGPRSModule()
{
	gprsWorkers.startUpTimer++;

	if (gprsWorkers.startUpTimer == (gprsSettings.startupDelay + 1))
	{
		write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_REQUEST_DISABLE_ECHO_STR, 
											sizeof(MOD_GPRS_COMMAND_REQUEST_DISABLE_ECHO_STR));
	}

	else if (gprsWorkers.startUpTimer == (gprsSettings.startupDelay + 2))
	{
		write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_SET_FUNCTIONALITY_STR, 
											sizeof(MOD_GPRS_COMMAND_SET_FUNCTIONALITY_STR));
	}

	else if (gprsWorkers.startUpTimer == (gprsSettings.startupDelay + 3))
	{
		write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_SET_GPRS_MODE_ON_STR, 
											sizeof(MOD_GPRS_COMMAND_SET_GPRS_MODE_ON_STR));
	}

	else if (gprsWorkers.startUpTimer == (gprsSettings.startupDelay + 10))
	{
		write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_SET_CONNECTION_TYPE_STR, 
											sizeof(MOD_GPRS_COMMAND_SET_CONNECTION_TYPE_STR));
	}

	else if (gprsWorkers.startUpTimer == (gprsSettings.startupDelay + 11))
	{
		write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_SET_APN_STR, 
											sizeof(MOD_GPRS_COMMAND_SET_APN_STR));
	}

	else if (gprsWorkers.startUpTimer == (gprsSettings.startupDelay + 13))
	{	
		write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_REQUEST_IP_STR, 
											sizeof(MOD_GPRS_COMMAND_REQUEST_IP_STR));
	}

	else if (gprsWorkers.startUpTimer > (gprsSettings.startupDelay + 13))
	{
		gprsWorkers.startUpTimer 	= 0;
		gprsWorkers.startUp			= false;
	}
}

/*-----------------------------------------------------------------------------
 * GPRS mutex functions
 *
 *---------------------------------------------------------------------------*/
int mod_gprs_MutexInit()
{
	int returnCode = RETURN_CODE_FAIL;

	gprsMutex = new pthread_mutex_t;

	if (gprsMutex != NULL)
	{
		if (pthread_mutex_init(gprsMutex, NULL) == 0)
		{
			returnCode = RETURN_CODE_OK;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_gprs_MutexDestroy()
{
	if (gprsMutex != NULL)
	{
		pthread_mutex_destroy(gprsMutex);

		delete gprsMutex;
		gprsMutex = NULL;
	}
}

/*---------------------------------------------------------------------------*/
bool mod_gprs_CheckSerialReply()
{
	pthread_mutex_lock(gprsMutex);
	bool isEmpty = gprsWorkers.replyList.empty();
	pthread_mutex_unlock(gprsMutex);

	return isEmpty;
}

/*---------------------------------------------------------------------------*/
String mod_gprs_GetSerialReply()
{
	String reply;
	reply.clear();

	pthread_mutex_lock(gprsMutex);
	if (!gprsWorkers.replyList.empty())
	{
		reply = gprsWorkers.replyList.front();
		gprsWorkers.replyList.pop_front();
	}
	pthread_mutex_unlock(gprsMutex);

	return reply;
}

/*---------------------------------------------------------------------------*/
void mod_gprs_SetSerialReply(const String &reply)
{
	pthread_mutex_lock(gprsMutex);
	gprsWorkers.replyList.push_back(reply);
	pthread_mutex_unlock(gprsMutex);
}

/*-----------------------------------------------------------------------------
 * GPRS timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_gprs_Timer1HzEvent(int)
{
	gprsWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_gprs_Timer1HzHandler()
{
	if (gprsWorkers.timer1HzTrigger)
	{
		gprsWorkers.timer1HzTrigger = false;

		if (gprsWorkers.startUp)
		{
			mod_gprs_InitializeGPRSModule();
		}

		else
		{
			if (++gprsWorkers.locationUpdateTimer >= gprsSettings.locationUpdateInterval)
			{
				gprsWorkers.locationUpdateTimer = 0;
				write(gprsWorkers.serialConnection, MOD_GPRS_COMMAND_REQUEST_LOCATION_STR, 
													sizeof(MOD_GPRS_COMMAND_REQUEST_LOCATION_STR));
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_gprs_ActionGPRSTest(MQTTMessage &mqttMessage)
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
	//{	LIB_MQTT_TOPIC_RESOURCE_GPRS_TEST	,	mod_gprs_ActionGPRSTest		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_gprs_RegisterMQTTActions()
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
 * GPRS jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_gprs_SettingsTable[] =
{
	{	MOD_GPRS_SETTING_SERIAL_PORT_STR				,	mod_gprs_SetSerialPort				},
	{	MOD_GPRS_SETTING_SERIAL_BAUDRATE_STR			,	mod_gprs_SetSerialBaudrate			},
	{	MOD_GPRS_SETTING_LOCATION_UPDATE_INTERVAL_STR	,	mod_gprs_SetLocationUpdateInterval	},
	{	MOD_GPRS_SETTING_STARTUP_DELAY_STR				,	mod_gprs_SetStartUpDelay			}
	
};

/*-----------------------------------------------------------------------------
 * GPRS settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_gprs_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_gprs_SettingsTable) /
									  sizeof(mod_gprs_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_gprs_SettingsTable[index].settingsName) &&
				(mod_gprs_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_gprs_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * GPRS pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_gprs_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */
	gprsSettings.serialPort 			= MOD_INITIAL_SERIAL_PORT_STR;
	gprsSettings.serialBaudrate 		= MOD_INITIAL_SERIAL_BAUDRATE;
	
	gprsSettings.locationUpdateInterval = MOD_INITIAL_LOCATION_UPDATE_INTERVAL;
	gprsSettings.startupDelay			= MOD_INITIAL_STARTUP_DELAY;

	/* Initialise the workers */
	gprsWorkers.timer1HzId 			= 0;
	gprsWorkers.timer1HzTrigger 	= false;

	gprsWorkers.serialConnection 	= 0;
	gprsWorkers.serialReadDelay		= 2;

	gprsWorkers.locationUpdateTimer = 0;
	gprsWorkers.replyTimer			= 0;

	gprsWorkers.replyList.clear();
	gprsWorkers.getReply 			= true;

	gprsWorkers.startUp				= true;
	gprsWorkers.startUpTimer		= 0;

	gprsWorkers.locationData.latitude.clear();
	gprsWorkers.locationData.longitude.clear();
	gprsWorkers.locationData.accuracy.clear();
}

/*-----------------------------------------------------------------------------
 * GPRS post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_gprs_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_GPRS_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_gprs_Timer1HzEvent,
										 &gprsWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_gprs_RegisterMQTTActions();

		returnCode = lib_serial_OpenConnection(gprsWorkers.serialConnection,
											   gprsSettings.serialPort,
											   gprsSettings.serialBaudrate);
		
		returnCode = mod_gprs_MutexInit();

		if (returnCode == RETURN_CODE_OK)
		{
			std::thread serialReplyHandler(mod_gprs_ReadFromSerialConnection);

			serialReplyHandler.detach();
		}

		return returnCode;
	}

	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}
/*-----------------------------------------------------------------------------
 * GPRS module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_gprs_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */
	close(gprsWorkers.serialConnection);

	gprsWorkers.getReply = false;

	mod_gprs_MutexDestroy();

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * GPRS mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_gprs_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_gprs_Timer1HzHandler();

		mod_gprs_ProcessSerialMessage();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/