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
 * Gps include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_gps_imp.h" 

/*-----------------------------------------------------------------------------
 * Gps settings and workers
 *
 *---------------------------------------------------------------------------*/
GpsSettings gpsSettings;
GpsWorkers  gpsWorkers;

/*---------------------------------------------------------------------------*/
static pthread_mutex_t *gpsMutex;

/*-----------------------------------------------------------------------------
 * Gps prototype functions
 *
 *---------------------------------------------------------------------------*/
bool mod_gps_CheckSerialReply();
String mod_gps_GetSerialReply();
void mod_gps_SetSerialReply(const String &reply);

/*-----------------------------------------------------------------------------
 * Gps publish functions
 *
 *---------------------------------------------------------------------------*/
int mod_gps_PublishLocation()
{
	int returnCode = RETURN_CODE_FAIL;

	if ((gpsWorkers.gpsLocation.latitude != MOD_GPS_LATITUDE_DEGREE_MIN) &&
		(gpsWorkers.gpsLocation.longitude != MOD_GPS_LONGITUDE_DEGREE_MIN) &&
		(gpsWorkers.gpsLocation.accuracy != MOD_GPS_ACCURACY_MIN))
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%f %f %f", gpsWorkers.gpsLocation.latitude, 
									gpsWorkers.gpsLocation.longitude, 
									gpsWorkers.gpsLocation.accuracy);
		returnCode = lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_LOCATION, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Gps general functions
 *
 *---------------------------------------------------------------------------*/
int	mod_gps_ProcessMessageLocation(int &argumentCount, String arguments[])
{
	int returnCode = RETURN_CODE_FAIL;

	if(argumentCount == 15)
	{
		returnCode = RETURN_CODE_OK;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = RETURN_CODE_FAIL;

		if (arguments[MOD_GPS_ARGUMENT_LATITUDE].length() > 9)
		{
			String degreeStr = arguments[MOD_GPS_ARGUMENT_LATITUDE].substr(0, 2);
			String minutesStr = arguments[MOD_GPS_ARGUMENT_LATITUDE].substr(2, 8);

			double degree = ::atof(degreeStr.c_str());
			double minutes = ::atof(minutesStr.c_str());

			if((degree > MOD_GPS_LATITUDE_DEGREE_MIN) && (degree < MOD_GPS_LATITUDE_DEGREE_MAX) &&
				(minutes > MOD_GPS_MINUTES_MIN) && (minutes < MOD_GPS_MINUTES_MAX))
			{
				gpsWorkers.gpsLocation.latitude = (minutes / 60) + degree;

				returnCode = RETURN_CODE_OK;
			}
		}
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = RETURN_CODE_FAIL;

		if (arguments[MOD_GPS_ARGUMENT_LONGITUDE].length() > 10)
		{
			String degreeStr = arguments[MOD_GPS_ARGUMENT_LONGITUDE].substr(0, 3);
			String minutesStr = arguments[MOD_GPS_ARGUMENT_LONGITUDE].substr(3, 8);

			double degree = ::atof(degreeStr.c_str());
			double minutes = ::atof(minutesStr.c_str());

			if((degree > MOD_GPS_LONGITUDE_DEGREE_MIN) && (degree < MOD_GPS_LONGITUDE_DEGREE_MAX) &&
				(minutes > MOD_GPS_MINUTES_MIN) && (minutes < MOD_GPS_MINUTES_MAX))
			{
				gpsWorkers.gpsLocation.longitude = (minutes / 60) + degree;

				returnCode = RETURN_CODE_OK;
			}
		}
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = RETURN_CODE_FAIL;

		if (arguments[MOD_GPS_ARGUMENT_SATTALLITES].length() > 1)
		{
			int sattallites = ::atoi(arguments[MOD_GPS_ARGUMENT_SATTALLITES].c_str());

			if((sattallites > MOD_GPS_SATTALLITES_MIN) && (sattallites < MOD_GPS_SATTALLITES_MAX))
			{
				gpsWorkers.gpsLocation.sattallites 	= sattallites;

				returnCode = RETURN_CODE_OK;
			}
		}
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = RETURN_CODE_FAIL;

		if (arguments[MOD_GPS_ARGUMENT_ACCURACY].length() > 1)
		{
			double accuracy = ::atof(arguments[MOD_GPS_ARGUMENT_ACCURACY].c_str());

			if(accuracy > MOD_GPS_ACCURACY_MIN)
			{
				gpsWorkers.gpsLocation.accuracy = accuracy;

				returnCode = RETURN_CODE_OK;
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_gps_TokenizeSerialMessage(const String &message, 
                                    int &tokenAmount, 
                                    String tokens[], 
                                    char seperator)
{
	String token;
	String previousToken;

    std::istringstream arg(message);
    std::getline(arg, token, seperator);

	tokenAmount = 0;

    previousToken = tokens[tokenAmount++] = token;

    std::getline(arg, token, seperator);

    while (previousToken != token)
    {
        previousToken = tokens[tokenAmount++] = token;

		std::getline(arg, token, seperator);
    }
}

/*---------------------------------------------------------------------------*/
int	mod_gps_ProcessSerialMessage()
{
	int returnCode = RETURN_CODE_OK;

	String message;
	message.clear();

	if (!mod_gps_CheckSerialReply())
	{
		String messageArguments[50];
		int argumentCount = 0;

		message = mod_gps_GetSerialReply();

		mod_gps_TokenizeSerialMessage(message, 
									  argumentCount, 
									  messageArguments, 
									  ',');

		if (messageArguments[0] == "$GPGGA")
		{
			returnCode = mod_gps_ProcessMessageLocation(argumentCount, messageArguments);

			if ((returnCode == RETURN_CODE_OK) && (gpsSettings.locationUpdateInterval == 0))
			{
				returnCode = mod_gps_PublishLocation();

				if (gpsWorkers.locationCallback != NULL)
				{
					(gpsWorkers.locationCallback)(gpsWorkers.gpsLocation);
				}
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_gps_ReadFromSerialConnection()
{
	int returnCode = RETURN_CODE_OK;

	struct timespec sleepTimeRequest, sleepTimeRemain;
	sleepTimeRequest.tv_sec 	= 20000000 / 1000000000;
	sleepTimeRequest.tv_nsec 	= 20000000 % 1000000000;

	int messageLength = 0;

	String message, messageBuffer;
	messageBuffer.clear();
	message.clear();

	while (gpsWorkers.getReply)
	{
		returnCode = lib_serial_ReadFromConnection(gpsWorkers.serialConnection, 
												   messageLength, 
												   messageBuffer);

		if ((messageLength > 0) && (returnCode == RETURN_CODE_OK))
		{
			message.append(messageBuffer);
			messageBuffer.clear();

			messageLength = 0;
		}

		if (message.find("\n") != String::npos)
		{
			mod_gps_SetSerialReply(message);

			message.clear();
		}

		nanosleep(&sleepTimeRequest, &sleepTimeRemain);
	}
}

/*---------------------------------------------------------------------------*/
void mod_gps_RegisterLocationCallback(LocationCallback *locationCallback)
{
    gpsWorkers.locationCallback = locationCallback;
}

/*-----------------------------------------------------------------------------
 * Gps mutex functions
 *
 *---------------------------------------------------------------------------*/
int mod_gps_MutexInit()
{
	int returnCode = RETURN_CODE_FAIL;

	gpsMutex = new pthread_mutex_t;

	if (gpsMutex != NULL)
	{
		if (pthread_mutex_init(gpsMutex, NULL) == 0)
		{
			returnCode = RETURN_CODE_OK;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_gps_MutexDestroy()
{
	if (gpsMutex != NULL)
	{
		pthread_mutex_destroy(gpsMutex);

		delete gpsMutex;
		gpsMutex = NULL;
	}
}

/*---------------------------------------------------------------------------*/
bool mod_gps_CheckSerialReply()
{
	pthread_mutex_lock(gpsMutex);
	bool isEmpty = gpsWorkers.replyList.empty();
	pthread_mutex_unlock(gpsMutex);

	return isEmpty;
}

/*---------------------------------------------------------------------------*/
String mod_gps_GetSerialReply()
{
	String reply;
	reply.clear();

	pthread_mutex_lock(gpsMutex);
	if (!gpsWorkers.replyList.empty())
	{
		reply = gpsWorkers.replyList.front();
		gpsWorkers.replyList.pop_front();
	}
	pthread_mutex_unlock(gpsMutex);

	return reply;
}

/*---------------------------------------------------------------------------*/
void mod_gps_SetSerialReply(const String &reply)
{
	pthread_mutex_lock(gpsMutex);
	gpsWorkers.replyList.push_back(reply);
	pthread_mutex_unlock(gpsMutex);
}

/*-----------------------------------------------------------------------------
 * Gps timer functions
 *---------------------------------------------------------------------------*/
void mod_gps_Timer1HzEvent(int)
{
	gpsWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_gps_Timer1HzHandler()
{
	if (gpsWorkers.timer1HzTrigger)
	{
		gpsWorkers.timer1HzTrigger = false;

		if (gpsSettings.locationUpdateInterval > 0)
		{
			if (++gpsWorkers.locationUpdateTimer >= gpsSettings.locationUpdateInterval)
			{
				gpsWorkers.locationUpdateTimer = 0;
				mod_gps_PublishLocation();

				if (gpsWorkers.locationCallback != NULL)
				{
					(gpsWorkers.locationCallback)(gpsWorkers.gpsLocation);
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_gps_ActionGpsTest(MQTTMessage &mqttMessage)
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
	//{	LIB_MQTT_TOPIC_RESOURCE_GPS_TEST	,	mod_gps_ActionGpsTest		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_gps_RegisterMQTTActions()
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
 * Gps jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_gps_SettingsTable[] =
{
	{	MOD_GPS_SETTING_SERIAL_PORT_STR					,	mod_gps_SetSerialPort				},
	{	MOD_GPS_SETTING_SERIAL_BAUDRATE_STR				,	mod_gps_SetSerialBaudrate			},
	{	MOD_GPS_SETTING_LOCATION_UPDATE_INTERVAL_STR	,	mod_gps_SetLocationUpdateInterval	}
};

/*-----------------------------------------------------------------------------
 * Gps settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_gps_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_gps_SettingsTable) /
									  sizeof(mod_gps_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_gps_SettingsTable[index].settingsName) &&
				(mod_gps_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_gps_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Gps pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_gps_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */
	gpsSettings.serialPort 				= MOD_GPS_INITIAL_SERIAL_PORT_STR;
	gpsSettings.serialBaudrate 			= MOD_GPS_INITIAL_SERIAL_BAUDRATE;

	gpsSettings.locationUpdateInterval = MOD_GPS_INITIAL_LOCATION_UPDATE_INTERVAL;

	/* Initialise the workers */
	gpsWorkers.timer1HzId 			= 0;
	gpsWorkers.timer1HzTrigger 		= false;
	gpsWorkers.locationUpdateTimer 	= 0;

	gpsWorkers.serialConnection 	= 0;

	gpsWorkers.replyList.clear();
	gpsWorkers.getReply 			= true;

	gpsWorkers.gpsLocation.latitude 	= 0.0;
	gpsWorkers.gpsLocation.longitude 	= 0.0;
	gpsWorkers.gpsLocation.accuracy 	= 0.0;
	gpsWorkers.gpsLocation.sattallites 	= 0;

}

/*-----------------------------------------------------------------------------
 * Gps post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_gps_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_GPS_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_gps_Timer1HzEvent,
										 &gpsWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_gps_RegisterMQTTActions();

		returnCode = lib_serial_OpenConnection(gpsWorkers.serialConnection,
											   gpsSettings.serialPort,
											   gpsSettings.serialBaudrate,
											   false);

		returnCode = mod_gps_MutexInit();

		if (returnCode == RETURN_CODE_OK)
		{
			std::thread serialReplyHandler(mod_gps_ReadFromSerialConnection);

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
 * Gps module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_gps_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */
	close(gpsWorkers.serialConnection);

	gpsWorkers.getReply = false;

	mod_gps_MutexDestroy();

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Gps mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_gps_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_gps_Timer1HzHandler();

		mod_gps_ProcessSerialMessage();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/