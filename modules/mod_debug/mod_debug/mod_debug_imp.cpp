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
 * Debug include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_debug_imp.h" 

/*-----------------------------------------------------------------------------
 * Debug settings and workers
 *
 *---------------------------------------------------------------------------*/
DebugSettings debugSettings;
DebugWorkers  debugWorkers;

/*-----------------------------------------------------------------------------
 * Debug prototype functions
 *
 *---------------------------------------------------------------------------*/
void 	mod_debug_debugLog(uint32_t mask, LogInfo *log);
void 	mod_debug_Append(uint32_t mask, LogInfo *log);
String 	mod_debug_FormatMessage(LogInfo *log);
String 	mod_debug_FormatLogFilePath();
void 	mod_debug_PrintDebugMessage(String debugMessage);

/*-----------------------------------------------------------------------------
 * Debug general functions
 *
 *---------------------------------------------------------------------------*/
void mod_debug_SetDebugMode(DebugLogMode debugMode)
{
	debugWorkers.debugMode = (DebugLogMode)debugMode;
}

/*---------------------------------------------------------------------------*/
void mod_debug_debugLog(DebugLogMode mask, LogInfo *log)
{
	if (debugWorkers.initialized)
	{
		if (mask & debugSettings.logMode)
		{
			String debugMessage = mod_debug_FormatMessage(log);
			String logFilePath  = mod_debug_FormatLogFilePath();

			int returnCode = lib_file_CheckFile(logFilePath);

			if (returnCode == RETURN_CODE_ERR_FILE_READ_FAILED)
			{

				returnCode = lib_file_CreateFile(logFilePath);

				if (returnCode == RETURN_CODE_OK)
				{					
					String logHeader = MOD_DEBUG_MESSAGE_FILE_HEADER_STR;
					logHeader.append(lib_time_GetDateString());
					logHeader.append(MOD_DEBUG_MESSAGE_END_OF_LINE_STR);
					logHeader.append(MOD_DEBUG_MESSAGE_END_OF_LINE_STR);

					returnCode = lib_file_AppendToFile(logFilePath, logHeader);

					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "log file created");
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				returnCode = lib_file_AppendToFile(logFilePath, debugMessage);
			}
		}

		if (mask & debugWorkers.debugMode)
		{
			String debugMessage = mod_debug_FormatMessage(log);

			mod_debug_PrintDebugMessage(debugMessage);
		}

		if ((debugWorkers.channelStarted) && (mask & debugSettings.logMode) &&
			!(mask & MOD_DEBUG_LOG_MODE_TYPE_MESSAGING))
		{
			lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_DEBUGGING, 
									log->message,
									LIB_MQTT_TOPIC_DEST_WEB_API_STR);
		}
	}

	else
	{
		mod_debug_Append(mask, log);
	}	
}

/*---------------------------------------------------------------------------*/
void mod_debug_debugPrintf(DebugLogMode mask, const char *funct, const char* format, ...)
{
	LogInfo logInfo;

	if (debugWorkers.initialized)
	{
		if ((mask & debugSettings.logMode) || 
			(mask & debugWorkers.debugMode))
		{
			char messageBuffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			va_list arglist;
			va_start(arglist, format);
       		vsnprintf(messageBuffer, 
                  	  MOD_DEBUG_MESSAGE_LENGTH_MAX, 
                      format, 
                  	  arglist);
        	va_end(arglist);

			logInfo.function = String(funct);
			logInfo.message  = String(messageBuffer);

			String debugMessage = mod_debug_FormatMessage(&logInfo);

			if (mask & debugSettings.logMode)
			{
				String logFilePath = mod_debug_FormatLogFilePath();

				int returnCode = lib_file_CheckFile(logFilePath);

				if (returnCode == RETURN_CODE_ERR_FILE_READ_FAILED)
				{
					returnCode = lib_file_CreateFile(logFilePath);

					if (returnCode == RETURN_CODE_OK)
					{					
						String logHeader = MOD_DEBUG_MESSAGE_FILE_HEADER_STR;
						logHeader.append(lib_time_GetDateString());
						logHeader.append(MOD_DEBUG_MESSAGE_END_OF_LINE_STR);
						logHeader.append(MOD_DEBUG_MESSAGE_END_OF_LINE_STR);

						returnCode = lib_file_AppendToFile(logFilePath, logHeader);

						debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "log file created");
					}
				}

				if (returnCode == RETURN_CODE_OK)
				{
					returnCode = lib_file_AppendToFile(logFilePath, debugMessage);
				}
			}

			if (mask & debugWorkers.debugMode)
			{
				mod_debug_PrintDebugMessage(debugMessage);
			}

			if ((debugWorkers.channelStarted) && (mask & debugSettings.logMode) &&
				!(mask & MOD_DEBUG_LOG_MODE_TYPE_MESSAGING))
			{
				lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_DEBUGGING, 
										logInfo.message,
										LIB_MQTT_TOPIC_DEST_WEB_API_STR);
			}
		}
	}

	else
	{
		char messageBuffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		va_list arglist;
		va_start(arglist, format);
		vsnprintf(messageBuffer, 
					MOD_DEBUG_MESSAGE_LENGTH_MAX, 
					format, 
					arglist);
		va_end(arglist);

		logInfo.function = String(funct);
		logInfo.message  = String(messageBuffer);

		mod_debug_Append(mask, &logInfo);
	}	
}

/*---------------------------------------------------------------------------*/
void mod_debug_Append(uint32_t mask, LogInfo *log)
{
	LogMessage logMessage;

	logMessage.mask	= mask;
	logMessage.logInfo.message = log->message;
	logMessage.logInfo.function = log->function;

	debugWorkers.logMessageList.push_back(logMessage);
}

/*---------------------------------------------------------------------------*/
String mod_debug_FormatMessage(LogInfo *log)
{
	String debugMessage;

	debugMessage.append(lib_time_GetDateTimeString());
	debugMessage.append(MOD_DEBUG_MESSAGE_TAB_STR);
	debugMessage.append(log->function);
	debugMessage.append(MOD_DEBUG_MESSAGE_TAB_STR);

	if ((log->function).length() <= 8)
	{
		debugMessage.append(MOD_DEBUG_MESSAGE_TAB_STR);
	}

	if ((log->function).length() <= 15)
	{
		debugMessage.append(MOD_DEBUG_MESSAGE_TAB_STR);
	}

	if ((log->function).length() <= 23)
	{
		debugMessage.append(MOD_DEBUG_MESSAGE_TAB_STR);
	}

	if ((log->function).length() <= 31)
	{
		debugMessage.append(MOD_DEBUG_MESSAGE_TAB_STR);
	}

	debugMessage.append(MOD_DEBUG_MESSAGE_KEY_STR);
	debugMessage.append(log->message);
	debugMessage.append(MOD_DEBUG_MESSAGE_END_OF_LINE_STR);
	
	return debugMessage;
}

/*---------------------------------------------------------------------------*/
String mod_debug_FormatLogFilePath()
{
	String logFilePath;

	logFilePath.append(debugSettings.logDirectory);
	logFilePath.append(MOD_DEBUG_MESSAGE_FILE_PREFIX_STR);
	logFilePath.append(lib_time_GetDateString());
	logFilePath.append(MOD_DEBUG_MESSAGE_FILE_SUFIX_STR);

	return logFilePath;
}
/*---------------------------------------------------------------------------*/
void mod_debug_PrintDebugMessage(String debugMessage)
{
	if(debugMessage.length() != 0)
	{
		std::cout << debugMessage;
	}
}

/*-----------------------------------------------------------------------------
 * Debug timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_debug_Timer1HzEvent(int)
{
	debugWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_debug_Timer1HzHandler()
{
	if (debugWorkers.timer1HzTrigger)
	{
		debugWorkers.timer1HzTrigger = false;

		if (debugWorkers.channelStarted)
		{
			if (debugWorkers.channelTimer++ >= debugSettings.channelTimeout)
			{
				debugWorkers.channelStarted = false;
				debugWorkers.channelTimer 	= 0;

				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "debugging channel closed");
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_debug_ActionDebugging(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	if (mqttMessage.messageData == MOD_DEBUG_CHANNEL_START)
	{
		if (!debugSettings.channelEnabled)
		{
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "debug channel not enabled");

			lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_DEBUGGING, 
									"debug channel not enabled",
									LIB_MQTT_TOPIC_DEST_WEB_API_STR);
		
			return RETURN_CODE_OK;
		}

		debugWorkers.channelStarted = true;
		debugWorkers.channelTimer 	= 0;

		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "debug channel started");
	}

	else if (mqttMessage.messageData == MOD_DEBUG_CHANNEL_PING)
	{
		debugWorkers.channelTimer = 0;
	}

	else if (mqttMessage.messageData == MOD_DEBUG_CHANNEL_STOP)
	{
		debugWorkers.channelStarted = false;
		debugWorkers.channelTimer 	= 0;

		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "debug channel stopped");
	}

	else
	{
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, "debug action invalid");
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{
	{ LIB_MQTT_TOPIC_RESOURCE_DEBUGGING		, mod_debug_ActionDebugging		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_debug_RegisterMQTTActions()
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
 * Debug jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_debug_SettingsTable[] =
{
	{	MOD_DEBUG_SETTING_LOG_MODE_STR			,	mod_debug_SetLoggingMode	},
	{	MOD_DEBUG_SETTING_LOG_DIRECTORY_STR		,	mod_debug_SetLogDirectory	},
	{	MOD_DEBUG_SETTING_CHANNEL_ENABLED_STR	,	mod_debug_SetChannelEnabled	},
	{	MOD_DEBUG_SETTING_CHANNEL_TIMEOUT_STR	,	mod_debug_SetChannelTimeout	}
};

/*-----------------------------------------------------------------------------
 * Debug settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_debug_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_debug_SettingsTable) /
									  sizeof(mod_debug_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_debug_SettingsTable[index].settingsName) &&
				(mod_debug_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_debug_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Debug pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_debug_PreInitialisation()
{
	/* Initialise the settings */
	debugSettings.logMode			= (DebugLogMode)MOD_DEBUG_INITIAL_LOG_MODE_STR;
	debugSettings.logDirectory		= MOD_DEBUG_INITIAL_LOG_DIRECTORY_STR;
	debugSettings.channelEnabled	= MOD_DEBUG_INITIAL_CHANNEL_ENABLED_STR;
	debugSettings.channelTimeout	= MOD_DEBUG_INITIAL_CHANNEL_TIMEOUT_STR;

	/* Initialise the workers */
	debugWorkers.timer1HzId 		= 0;
	debugWorkers.timer1HzTrigger 	= false;

	debugWorkers.debugMode			= (DebugLogMode)MOD_DEBUG_LOG_MODE_TYPE_NONE;
	debugWorkers.initialized		= false;
	debugWorkers.logMessageList.clear();

	debugWorkers.channelStarted		= false;
	debugWorkers.channelTimer		= 0;


	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
}

/*-----------------------------------------------------------------------------
 * Debug post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_debug_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		mod_debug_RegisterMQTTActions();

		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_DEBUG_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_debug_Timer1HzEvent,
										 &debugWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		if (debugSettings.logMode != 0)
		{
			returnCode = lib_file_CheckFile(debugSettings.logDirectory);

			if (returnCode != RETURN_CODE_OK)
			{
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, "error: log directory invalid");
			}
		}

		debugWorkers.initialized = true;

		for (LogMessageList::const_iterator logMessageIt = debugWorkers.logMessageList.begin();
		 	 logMessageIt != debugWorkers.logMessageList.end(); logMessageIt++)
		{
			uint32_t mask = logMessageIt->mask;

			LogInfo logInfo;
			logInfo.function = logMessageIt->logInfo.function;
			logInfo.message = logMessageIt->logInfo.message;

			mod_debug_debugLog(mask, &logInfo);
		}

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}
/*-----------------------------------------------------------------------------
 * Debug module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_debug_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Debug mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_debug_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

		mod_debug_Timer1HzHandler();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/