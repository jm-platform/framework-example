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
 * Puppet Device include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_puppet_device_imp.h" 

/*-----------------------------------------------------------------------------
 * Puppet Device settings and workers
 *
 *---------------------------------------------------------------------------*/
PuppetDeviceSettings puppetDeviceSettings;
PuppetDeviceWorkers  puppetDeviceWorkers;

RF24 nrfModule(MOD_PUPPET_DEVICE_BASE_CE_PIN, 0);

/*-----------------------------------------------------------------------------
 * Puppet Device prototype functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Puppet Device functions
 *
 *---------------------------------------------------------------------------*/
int mod_puppet_device_WriteOnlineState(const String &filePath, 
									   const bool state, 
									   const int sensorIndex,
									   const int &sensorId)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_new_object();

	returnCode = lib_file_CheckFile(filePath);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = lib_json_ReadJsonFile(&jsonObject, filePath);

		if (returnCode == RETURN_CODE_OK)
		{
			json_object *listObject = json_object_object_get(jsonObject, 
													 		 MOD_PUPPET_DEVICE_VALUE_SENSOR_LIST);

			if ((listObject != NULL) && (json_object_get_type(listObject) == json_type_array))
			{
				int deviceAmount = json_object_array_length(listObject);
				for (int index = 0; index < deviceAmount; index++)
				{
					if (index == sensorIndex)
					{
						
						json_object *sensorObject = json_object_array_get_idx(listObject, index);
						json_object *statusObject = json_object_new_boolean(state);

						if ((sensorObject != NULL) && 
							(json_object_get_type(statusObject) == json_type_boolean))
						{
							int result = json_object_object_add(sensorObject, 
																MOD_PUPPET_DEVICE_VALUE_ONLINE, 
																statusObject);

							if (result == 0)
							{
								json_object *newObject = json_object_new_object();

								result = json_object_object_add(newObject, 
																MOD_PUPPET_DEVICE_VALUE_SENSOR_LIST, 
																listObject);

								if (result == 0)
								{
									returnCode = lib_json_WriteJsonFile(filePath, newObject);
								}
							}
						}
					}
				}		

				if (returnCode == RETURN_CODE_OK)
				{
					char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
					sprintf(buffer, "puppet device ID %d is \"%s\"", sensorId, state ? "online" : "offline");
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
				}

				else
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
											   "Failed to write online state to sensor info file");
				}
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_puppet_device_ReadSensorInfo(const String &filePath)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_new_object();

	returnCode = lib_file_CheckFile(filePath);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = lib_json_ReadJsonFile(&jsonObject, filePath);

		if (returnCode == RETURN_CODE_OK)
		{
			json_object *listObject = json_object_object_get(jsonObject, 
													 		 MOD_PUPPET_DEVICE_VALUE_SENSOR_LIST);

			if (listObject != NULL)
			{
				if (json_object_get_type(listObject) == json_type_array)
				{
					puppetDeviceWorkers.puppetDeviceAmount = json_object_array_length(listObject);

					if (puppetDeviceWorkers.puppetDeviceAmount <= MOD_PUPPET_DEVICE_PUPPET_DEVICE_AMOUNT_MAX)
					{
						for (int index = 0; index < puppetDeviceWorkers.puppetDeviceAmount; index++)
						{
							json_object *sensorObject = json_object_array_get_idx(listObject, index);

							int sensorId;
							
							returnCode = lib_json_ReadJsonInteger(sensorId, 
																MOD_PUPPET_DEVICE_VALUE_ID,
																sensorObject);

							if (returnCode == RETURN_CODE_OK)
							{
								String 	name;
								bool	online;

								returnCode = lib_json_ReadJsonString(name, 
																	MOD_PUPPET_DEVICE_VALUE_NAME,
																	sensorObject);

								if (returnCode == RETURN_CODE_OK)
								{
									puppetDeviceWorkers.puppetDeviceList[index].active = true;
									puppetDeviceWorkers.puppetDeviceList[index].online = false;
									puppetDeviceWorkers.puppetDeviceList[index].name = name;
									puppetDeviceWorkers.puppetDeviceList[index].id = sensorId;
									puppetDeviceWorkers.puppetDeviceList[index].address += sensorId;
								}
								
								returnCode = lib_json_ReadJsonBoolean(online, 
																	MOD_PUPPET_DEVICE_VALUE_ONLINE,
																	sensorObject);

								if ((returnCode == RETURN_CODE_OK) && (online))
								{
									returnCode = mod_puppet_device_WriteOnlineState(filePath,
																					MOD_PUPPET_DEVICE_STATE_OFFLINE,
																					index,
																					sensorId);
								}
							}
						}
					}

					else
					{
						debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
												"Error: to many sensors defined in sensor info");

						returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
					}	
				}			

				if (returnCode == RETURN_CODE_OK)
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
											   "Succesfully read the sensor info file");
				}

				else
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
											   "Failed to read the sensor info file");
				}
			}
		}
	}

	else
	{
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
									"Error: sensor info file does not exist");
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_puppet_device_RegisterMessageAction(const uint8_t &address, 
											 NRFMessageCallBack* messageCallback)
{
	NRFMessageAction messageAction;
	messageAction.address 			= address;
	messageAction.messageCallback	= messageCallback;

	puppetDeviceWorkers.messageActionList.push_back(messageAction);

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "NRF callback registered for address \"%d\"", address);
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
}
 
/*---------------------------------------------------------------------------*/
int mod_puppet_device_ReadConnections()
{
	int returnCode =  RETURN_CODE_OK;

	uint8_t pipe;

	if (nrfModule.available(&pipe)) 
	{                        
		uint8_t messageByteSize = nrfModule.getPayloadSize();

		if (messageByteSize == sizeof(uint64_t))
		{
			uint64_t 	dataBuffer;
			float 		message;

			nrfModule.read(&dataBuffer, messageByteSize);
			uint8_t address = (dataBuffer >> MOD_PUPPET_DEVICE_BITSHIFT_MESSAGE_ADDRESS) & 
								MOD_PUPPET_DEVICE_READ_MESSAGE_MASK_ADDRESS;
			uint32_t messageBuffer = dataBuffer & MOD_PUPPET_DEVICE_READ_MESSAGE_MASK_DATA;
			memcpy(&message, &messageBuffer, sizeof(float));

			if (!puppetDeviceWorkers.puppetDeviceList[pipe].online)
			{
				puppetDeviceWorkers.puppetDeviceList[pipe].online = true;

				returnCode = mod_puppet_device_WriteOnlineState(puppetDeviceSettings.puppetDevicesPath,
																MOD_PUPPET_DEVICE_STATE_ONLINE,
																pipe,
																puppetDeviceWorkers.puppetDeviceList[pipe].id);
			}

			else
			{
				puppetDeviceWorkers.puppetDeviceList[pipe].connectionTimer = 
					puppetDeviceSettings.connectionTimeout;
			}

			for (MessageActionList::const_iterator messageActionListIt = 
				 puppetDeviceWorkers.messageActionList.begin();
		 		 messageActionListIt != puppetDeviceWorkers.messageActionList.end(); 
				 messageActionListIt++)
			{
				if (messageActionListIt->address == address)
				{
					(*messageActionListIt->messageCallback)(puppetDeviceWorkers.puppetDeviceList[pipe].id,
															message);
				}
			}

			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "NRF message received: pipe = %d, address = %d, data = %.1f", 
					pipe, address, message);
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);
		}

		else
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "NRF message invalid size : %d bytes", messageByteSize);
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);
		}          
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_puppet_device_SendMessage(const uint8_t &address, const float &data)
{
	if ((puppetDeviceSettings.rolePuppet == MOD_PUPPET_DEVICE_ROLE_MANAGER_STR) ||
		(!puppetDeviceSettings.rfEnabled))
	{
		return RETURN_CODE_FAIL;
	}
	

	uint64_t message 	= 0;
	uint64_t dataBuffer	= 0;

	memcpy(&dataBuffer, &data, sizeof(float));

	message = (message & MOD_PUPPET_DEVICE_SEND_MESSAGE_MASK_DATA) | dataBuffer;
	message = (message & MOD_PUPPET_DEVICE_SEND_MESSAGE_MASK_ADDRESS) | 
				((uint64_t)address << MOD_PUPPET_DEVICE_BITSHIFT_MESSAGE_ADDRESS);

	if (nrfModule.write(&message, sizeof(uint64_t))) 
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "NRF message send \"0x%llX\"", message);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);

		return RETURN_CODE_OK;
	}

	else
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "Failed to send NRF message \"0x%llX\"", message);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);

		return RETURN_CODE_FAIL;
	}
}

/*---------------------------------------------------------------------------*/
int mod_puppet_device_GetPuppetDeviceId()
{
	if (puppetDeviceSettings.rolePuppet != MOD_PUPPET_DEVICE_ROLE_MANAGER_STR)
	{
		return (puppetDeviceSettings.puppetAddress - MOD_PUPPET_DEVICE_BASE_ADDRESS);
	}
	
	return RETURN_CODE_FAIL;
}

/*-----------------------------------------------------------------------------
 * Puppet Device timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_puppet_device_Timer1HzEvent(int)
{
	puppetDeviceWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_puppet_device_Timer1HzHandler()
{
	if (puppetDeviceWorkers.timer1HzTrigger)
	{
		puppetDeviceWorkers.timer1HzTrigger = false;

		if (puppetDeviceSettings.rolePuppet == MOD_PUPPET_DEVICE_ROLE_TEST_STR)
		{
			mod_puppet_device_SendMessage(MOD_PUPPET_DEVICE_NRF_ADDRESS_TEST,
										  puppetDeviceWorkers.dataOut);

			puppetDeviceWorkers.dataOut += 0.1;
		}

		for (int index = 0; index < MOD_PUPPET_DEVICE_PUPPET_DEVICE_AMOUNT_MAX; index++)
		{
			if (puppetDeviceWorkers.puppetDeviceList[index].online)
			{
				if(--puppetDeviceWorkers.puppetDeviceList[index].connectionTimer == 0)
				{
					puppetDeviceWorkers.puppetDeviceList[index].online = false;

					mod_puppet_device_WriteOnlineState(puppetDeviceSettings.puppetDevicesPath,
													   MOD_PUPPET_DEVICE_STATE_OFFLINE,
													   index,
													   puppetDeviceWorkers.puppetDeviceList[index].id);
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * MQTT Client message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_puppet_device_RegisterMQTTActions()
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
 * Puppet Device jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_puppet_device_SettingsTable[] =
{
	{	MOD_PUPPET_DEVICE_SETTING_SENSOR_INFO_PATH_STR		, mod_puppet_device_SetSensorInfoPath	},
	{	MOD_PUPPET_DEVICE_SETTING_PRINT_NRF_DETAILS_STR		, mod_puppet_device_SetPrintNRFDetails	},
	{	MOD_PUPPET_DEVICE_SETTING_SET_CHANNEL_STR			, mod_puppet_device_SetRFChannel		},
	{	MOD_PUPPET_DEVICE_SETTING_SET_POWER_STR				, mod_puppet_device_SetRFPowerLevel		},
	{	MOD_PUPPET_DEVICE_SETTING_PUPPET_DEVICE_ROLE_STR	, mod_puppet_device_SetPuppetDeviceRole	},
	{	MOD_PUPPET_DEVICE_SETTING_SET_TIMEOUT_STR			, mod_puppet_device_SetTimeout			},
	{	MOD_PUPPET_DEVICE_SETTING_SET_ENABLE_RF_STR			, mod_puppet_device_SetEnableRF			},
	{	MOD_PUPPET_DEVICE_SETTING_SET_NRF_MODE_STR			, mod_puppet_device_SetNRFMode			}
};

/*-----------------------------------------------------------------------------
 * Puppet Device settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_puppet_device_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_puppet_device_SettingsTable) /
									  sizeof(mod_puppet_device_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_puppet_device_SettingsTable[index].settingsName) &&
				(mod_puppet_device_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_puppet_device_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Puppet Device pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_puppet_device_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");

	/* Initialise the settings */
	puppetDeviceSettings.puppetDevicesPath 	= MOD_PUPPET_DEVICE_INITIAL_SENSOR_INFO_PATH_STR;
	puppetDeviceSettings.printNRFDetails	= false;
	puppetDeviceSettings.rfChannel			= MOD_PUPPET_DEVICE_INITIAL_RF_CHANNEL;
	puppetDeviceSettings.rfPowerLevel		= RF24_PA_MIN;
	puppetDeviceSettings.rolePuppet			= MOD_PUPPET_DEVICE_ROLE_MANAGER_STR;
	puppetDeviceSettings.puppetAddress		= MOD_PUPPET_DEVICE_BASE_ADDRESS;
	puppetDeviceSettings.connectionTimeout	= MOD_PUPPET_DEVICE_INITIAL_CONNECTION_TIMOUT;
	puppetDeviceSettings.rfEnabled			= MOD_PUPPET_DEVICE_INITIAL_ENABLE_RF;
	puppetDeviceSettings.nrfMode			= nrfModeNone;

	/* Initialise the workers */
	puppetDeviceWorkers.timer1HzId 			= 0;
	puppetDeviceWorkers.timer1HzTrigger 	= false;
	puppetDeviceWorkers.puppetDeviceAmount 	= 0;
	puppetDeviceWorkers.dataOut				= 0.0;

	puppetDeviceWorkers.messageActionList.clear();

	for (int index = 0; index < MOD_PUPPET_DEVICE_PUPPET_DEVICE_AMOUNT_MAX; index++)
	{
		puppetDeviceWorkers.puppetDeviceList[index].active 			= false;
		puppetDeviceWorkers.puppetDeviceList[index].online 			= false;
		puppetDeviceWorkers.puppetDeviceList[index].address 		= MOD_PUPPET_DEVICE_BASE_ADDRESS;
		puppetDeviceWorkers.puppetDeviceList[index].id 				= false;
		puppetDeviceWorkers.puppetDeviceList[index].connectionTimer	= 0;
		puppetDeviceWorkers.puppetDeviceList[index].name.clear();
	}

}

/*-----------------------------------------------------------------------------
 * Puppet Device post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_puppet_device_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_PUPPET_DEVICE_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_puppet_device_Timer1HzEvent,
										 &puppetDeviceWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_puppet_device_RegisterMQTTActions();

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_puppet_device_ReadSensorInfo(puppetDeviceSettings.puppetDevicesPath);
		}

		if (puppetDeviceSettings.rfEnabled)
		{
			if (!nrfModule.begin())
			{
				if (returnCode == RETURN_CODE_OK)
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
											"Fatal error: RF module not responding");

					returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				nrfModule.setPayloadSize(sizeof(uint64_t));
				nrfModule.setPALevel(puppetDeviceSettings.rfPowerLevel);
				nrfModule.setChannel(puppetDeviceSettings.rfChannel);
				nrfModule.setDataRate(RF24_250KBPS);

				if (puppetDeviceSettings.rolePuppet == MOD_PUPPET_DEVICE_ROLE_MANAGER_STR)
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
												"Starting as puppet device manager");

					for (int index = 0; index < MOD_PUPPET_DEVICE_PUPPET_DEVICE_AMOUNT_MAX; index++)
					{
						if (puppetDeviceWorkers.puppetDeviceList[index].active)
						{
							char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
							sprintf(buffer, 
									"Opening puppet device connection %d:\"0x%llX\"",
									index,
									puppetDeviceWorkers.puppetDeviceList[index].address);
							debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

							nrfModule.openReadingPipe(index, puppetDeviceWorkers.puppetDeviceList[index].address);
						}
					}
										
					nrfModule.startListening();
				}

				else
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
												"Starting as puppet device");

					nrfModule.openWritingPipe(puppetDeviceSettings.puppetAddress);
				}

				if (puppetDeviceSettings.printNRFDetails)
				{
					nrfModule.printPrettyDetails();
				}
			}
		}

		

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*-----------------------------------------------------------------------------
 * Puppet Device module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_puppet_device_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */
	if (puppetDeviceSettings.rfEnabled)
	{
		nrfModule.stopListening();
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Puppet Device mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_puppet_device_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_puppet_device_Timer1HzHandler();

		if ((puppetDeviceSettings.rolePuppet == MOD_PUPPET_DEVICE_ROLE_MANAGER_STR) &&
			(puppetDeviceSettings.rfEnabled))
		{
			returnCode = mod_puppet_device_ReadConnections();
		}

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/