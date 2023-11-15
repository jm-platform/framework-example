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
 * IO include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_io_imp.h" 

/*-----------------------------------------------------------------------------
 * IO settings and workers
 *
 *---------------------------------------------------------------------------*/
IOSettings ioSettings;
IOWorkers  ioWorkers;

/*---------------------------------------------------------------------------*/
static pthread_mutex_t *ioMutex;

/*-----------------------------------------------------------------------------
 * IO prototype functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * IO general functions
 *
 *---------------------------------------------------------------------------*/
int mod_io_ToggleOutput(const int outputPin)
{
	int returnCode = RETURN_CODE_FAIL;

	if (ioSettings.systemType == MOD_IO_SYSTEM_TYPE_RPI)
	{
		if ((outputPin >= MOD_IO_SETTING_RPI_GPIO_MIN) &&
			(outputPin <= MOD_IO_SETTING_RPI_GPIO_MAX))
		{
			bcm2835_gpio_fsel(outputPin, BCM2835_GPIO_FSEL_OUTP);

			uint8_t outputState = !bcm2835_gpio_lev(outputPin);
			bcm2835_gpio_write(outputPin, outputState);

			if (outputState == bcm2835_gpio_lev(outputPin))
			{
				returnCode = RETURN_CODE_OK;
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_io_SetOutput(const int outputPin, uint8_t outputState)
{
	int returnCode = RETURN_CODE_FAIL;

	if (ioSettings.systemType == MOD_IO_SYSTEM_TYPE_RPI)
	{
		if ((outputPin >= MOD_IO_SETTING_RPI_GPIO_MIN) &&
			(outputPin <= MOD_IO_SETTING_RPI_GPIO_MAX))
		{
			bcm2835_gpio_fsel(outputPin, BCM2835_GPIO_FSEL_OUTP);

			bcm2835_gpio_write(outputPin, outputState);

			if (outputState == bcm2835_gpio_lev(outputPin))
			{
				returnCode = RETURN_CODE_OK;
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_io_RegisterInputCallback(const int inputPin, InputSetCallback* inputCallback)
{
	InputAction inputAction;

	int returnCode = RETURN_CODE_FAIL;

	if ((inputPin >= MOD_IO_SETTING_RPI_GPIO_MIN) &&
		(inputPin <= MOD_IO_SETTING_RPI_GPIO_MAX))
	{
		inputAction.inputPin 		= inputPin;
		inputAction.inputCallback 	= inputCallback;

		ioWorkers.inputList.push_back(inputAction);

		bcm2835_gpio_fsel(inputPin, BCM2835_GPIO_FSEL_INPT);
		bcm2835_gpio_set_pud(inputPin, BCM2835_GPIO_PUD_UP);

		returnCode = RETURN_CODE_OK;

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "input %d is set", inputPin);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}	

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_io_InitializeInput(const int inputPin)
{
	int returnCode = RETURN_CODE_FAIL;

	if ((inputPin >= MOD_IO_SETTING_RPI_GPIO_MIN) &&
		(inputPin <= MOD_IO_SETTING_RPI_GPIO_MAX))
	{
		bcm2835_gpio_fsel(inputPin, BCM2835_GPIO_FSEL_INPT);
		bcm2835_gpio_set_pud(inputPin, BCM2835_GPIO_PUD_UP);

		returnCode = RETURN_CODE_OK;

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "input %d is set", inputPin);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}	

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * IO mutex functions
 *
 *---------------------------------------------------------------------------*/
int mod_io_MutexInit()
{
	int returnCode = RETURN_CODE_FAIL;

	ioMutex = new pthread_mutex_t;

	if (ioMutex != NULL)
	{
		if (pthread_mutex_init(ioMutex, NULL) == 0)
		{
			returnCode = RETURN_CODE_OK;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_io_MutexDestroy()
{
	if (ioMutex != NULL)
	{
		pthread_mutex_destroy(ioMutex);

		delete ioMutex;
		ioMutex = NULL;
	}
}

/*---------------------------------------------------------------------------*/
bool mod_io_CheckInputTriggers()
{
	pthread_mutex_lock(ioMutex);
	bool isEmpty = ioWorkers.inputTriggers.empty();
	pthread_mutex_unlock(ioMutex);

	return isEmpty;
}

/*---------------------------------------------------------------------------*/
int mod_io_GetInputTrigger()
{
	int inputTrigger = 0;

	pthread_mutex_lock(ioMutex);
	if (!ioWorkers.inputTriggers.empty())
	{
		inputTrigger = ioWorkers.inputTriggers.front();
		ioWorkers.inputTriggers.pop_front();
	}
	pthread_mutex_unlock(ioMutex);

	return inputTrigger;
}

/*---------------------------------------------------------------------------*/
void mod_io_SetInputTrigger(const int &inputTrigger)
{
	pthread_mutex_lock(ioMutex);
	ioWorkers.inputTriggers.push_back(inputTrigger);
	pthread_mutex_unlock(ioMutex);
}

/*---------------------------------------------------------------------------*/
bool mod_io_CheckOutputTrigger()
{
	pthread_mutex_lock(ioMutex);
	bool isEmpty = ioWorkers.outputTriggers.empty();
	pthread_mutex_unlock(ioMutex);

	return isEmpty;
}

/*---------------------------------------------------------------------------*/
void mod_io_SetOutputTrigger(const int outputPin, const int outputState)
{
	OutputAction outputAction;
	outputAction.outputPin		= outputPin;
	outputAction.outputState	= outputState;

	pthread_mutex_lock(ioMutex);
	ioWorkers.outputTriggers.push_back(outputAction);
	pthread_mutex_unlock(ioMutex);
}

/*---------------------------------------------------------------------------*/
OutputAction mod_io_GetOutputTrigger()
{
	OutputAction outputAction;
	outputAction.outputPin		= -1;
	outputAction.outputState	= -1;

	pthread_mutex_lock(ioMutex);
	if (!ioWorkers.outputTriggers.empty())
	{
		outputAction = ioWorkers.outputTriggers.front();
		ioWorkers.outputTriggers.pop_front();
	}
	pthread_mutex_unlock(ioMutex);

	return outputAction;
}

/*---------------------------------------------------------------------------*/
void mod_io_SetInputState(const int inputPin, const int inputState)
{
	if (inputPin > (MOD_IO_SETTING_RPI_GPIO_MAX + 1))
	{
		return;
	}
	
	pthread_mutex_lock(ioMutex);
	ioWorkers.inputState[inputPin] = inputState;
	pthread_mutex_unlock(ioMutex);
}

/*---------------------------------------------------------------------------*/
int mod_io_GetInputState(const int inputPin)
{
	if (inputPin > (MOD_IO_SETTING_RPI_GPIO_MAX + 1))
	{
		return RETURN_CODE_FAIL;
	}

	pthread_mutex_lock(ioMutex);
	int inputState = ioWorkers.inputState[inputPin];
	pthread_mutex_unlock(ioMutex);

	return inputState;
}

/*-----------------------------------------------------------------------------
 * IO mainloop functions
 *
 *---------------------------------------------------------------------------*/
int mod_io_InputHandler()
{
	int returnCode = RETURN_CODE_OK;

	int inputTrigger = mod_io_GetInputTrigger();

	if (inputTrigger != 0)
	{
		for (InputList::iterator inputListIt = ioWorkers.inputList.begin();
		 	 inputListIt != ioWorkers.inputList.end(); inputListIt++)
		{
			if (inputListIt->inputPin == inputTrigger)
			{
				(inputListIt->inputCallback)();
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_io_SetInputDetectionLoop()
{
	struct timespec sleepTimeRequest, sleepTimeRemain;
	sleepTimeRequest.tv_sec 	= MOD_IO_INPUT_LOOP_SLEEP_INTERVAL / 1000000000;
	sleepTimeRequest.tv_nsec 	= MOD_IO_INPUT_LOOP_SLEEP_INTERVAL % 1000000000;

	uint8_t inputState[MOD_IO_SETTING_RPI_GPIO_MAX + 1];
	memset(inputState, LOW, sizeof(inputState));

	int inputPinMin = 0, inputPinMax = 0;

	if (ioSettings.systemType == MOD_IO_SYSTEM_TYPE_RPI)
	{
		inputPinMin = MOD_IO_SETTING_RPI_GPIO_MIN;
		inputPinMax = MOD_IO_SETTING_RPI_GPIO_MAX;
	}

	else
	{
		ioWorkers.inputLoopEnabled = false;
	}

	while(ioWorkers.inputLoopEnabled)
	{
		for (int inputPin = inputPinMin; inputPin < inputPinMax; inputPin++)
		{
			if (bcm2835_gpio_lev(inputPin) == HIGH)
			{
				inputState[inputPin] = HIGH;
			}

			else if ((bcm2835_gpio_lev(inputPin) == LOW) &&
					 (inputState[inputPin] == HIGH))
			{
				inputState[inputPin] = LOW;

				if (mod_io_CheckInputTriggers())
				{
					mod_io_SetInputTrigger(inputPin);
				}
			}

			mod_io_SetInputState(inputPin, bcm2835_gpio_lev(inputPin));
		}

		nanosleep(&sleepTimeRequest, &sleepTimeRemain);
	}
}

/*---------------------------------------------------------------------------*/
int mod_io_OutputHandler()
{
	int returnCode = RETURN_CODE_OK;

	if (!mod_io_CheckOutputTrigger())
	{
		OutputAction outputAction = mod_io_GetOutputTrigger();

		returnCode = mod_io_SetOutput(outputAction.outputPin, outputAction.outputState);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * IO timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_io_Timer1HzEvent(int)
{
	ioWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_io_Timer1HzHandler()
{
	if (ioWorkers.timer1HzTrigger)
	{
		ioWorkers.timer1HzTrigger = false;
	}
}

/*-----------------------------------------------------------------------------
 * IO message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_io_ActionToggleOutput(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
								MOD_DEBUG_MESSAGE_EQUAL_STR, 
								mqttMessage.messageData.c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

	int outputPin = std::stoi(mqttMessage.messageData);

	returnCode = mod_io_ToggleOutput(outputPin);

	if (returnCode == RETURN_CODE_OK)
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "output %d is set", outputPin);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	else
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "gpio pin %d is invalid", outputPin);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_io_ActionSetOutput(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "received \'%s\' %s \"%s\"", mqttMessage.messageTopic.c_str(),
								MOD_DEBUG_MESSAGE_EQUAL_STR, 
								mqttMessage.messageData.c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

	String outputPinstr, outputStatestr;
	int outputPin = 0, outputState = 0;

	returnCode = lib_mqtt_GetDoubleMessageArguments(mqttMessage.messageData,
													outputPinstr,
													outputStatestr);

	if (returnCode == RETURN_CODE_OK)
	{
		outputPin = std::stoi(outputPinstr);
		outputState = std::stoi(outputStatestr);
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_io_SetOutput(outputPin, outputState);
	}

	if (returnCode == RETURN_CODE_OK)
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "output %d state = %s", outputPin, outputState? "high" : "low");
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	else
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "gpio pin %d is invalid", outputPin);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * IO message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{
	{	LIB_MQTT_TOPIC_RESOURCE_TOGGLE_OUTPUT	,	mod_io_ActionToggleOutput		},
	{	LIB_MQTT_TOPIC_RESOURCE_SET_OUTPUT		,	mod_io_ActionSetOutput			}
};

/*-----------------------------------------------------------------------------
 * IO Action register
 *
 *---------------------------------------------------------------------------*/
int mod_io_RegisterMQTTActions()
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
 * IO jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_io_SettingsTable[] =
{
	{	MOD_IO_SETTING_SYSTEM_TYPE_STR	,	mod_io_SetSystemType	}
};

/*-----------------------------------------------------------------------------
 * IO settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_io_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_io_SettingsTable) /
									  sizeof(mod_io_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_io_SettingsTable[index].settingsName) &&
				(mod_io_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_io_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * IO pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_io_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */
	ioSettings.systemType = MOD_IO_INITIAL_SYSTEM_TYPE_STR;

	/* Initialise the workers */
	ioWorkers.timer1HzId 		= 0;
	ioWorkers.timer1HzTrigger 	= false;

	ioWorkers.inputLoopEnabled 	= true;
	memset(ioWorkers.inputState, LOW, sizeof(ioWorkers.inputState));

	ioWorkers.inputList.clear();
	ioWorkers.inputTriggers.clear();
	ioWorkers.outputTriggers.clear();
}

/*-----------------------------------------------------------------------------
 * IO post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_io_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_IO_1HZ,
										 LIB_TIME_TIMER_1HZ_VALUE,
										 (void*)mod_io_Timer1HzEvent,
										 &ioWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_io_RegisterMQTTActions();
		}

		if (returnCode == RETURN_CODE_OK)
		{
			if (ioSettings.systemType == MOD_IO_SYSTEM_TYPE_RPI)
			{
				bcm2835_init();
			}
		}

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_io_MutexInit();

			if (returnCode == RETURN_CODE_OK)
			{
				std::thread inputLoop(mod_io_SetInputDetectionLoop);
				inputLoop.detach();
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
 * IO module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_io_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */
	ioWorkers.inputLoopEnabled = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	mod_io_MutexDestroy();

	if (ioSettings.systemType == MOD_IO_SYSTEM_TYPE_RPI)
	{
		bcm2835_gpio_clr_multi(MOD_IO_RPI_GPIO_MASK_ALL);
		bcm2835_close();
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * IO mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_io_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_io_Timer1HzHandler();

		returnCode = mod_io_InputHandler();

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_io_OutputHandler();
		}

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/