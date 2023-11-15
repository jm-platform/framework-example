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
 * Synthesizer include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_synthesizer_imp.h" 

/*-----------------------------------------------------------------------------
 * Synthesizer settings and workers
 *
 *---------------------------------------------------------------------------*/
SynthesizerSettings synthesizerSettings;
SynthesizerWorkers  synthesizerWorkers;

/*-----------------------------------------------------------------------------
 * Synthesizer prototype functions
 *
 *---------------------------------------------------------------------------*/
void mod_synthesizer_InitializeInstrument(Instrument instrument);

/*-----------------------------------------------------------------------------
 * Synthesizer JSON functions
 *
 *---------------------------------------------------------------------------*/
int mod_synthesizer_ReadKeyType(json_object *instrumentObject, KeyType &keyType)
{
	String keyTypeStr;

	int returnCode = lib_json_ReadJsonString(keyTypeStr, 
											 MOD_SYNTHESIZER_NAME_KEY_TYPE_STR,
											 instrumentObject);

	if (keyTypeStr == MOD_SYNTHESIZER_VALUE_PITCH_STR)
	{
		keyType = keyTypePitch;
	}

	else if (keyTypeStr == MOD_SYNTHESIZER_VALUE_NOTES_STR)
	{
		keyType = keyTypeNotes;
	}

	else if (keyTypeStr == MOD_SYNTHESIZER_VALUE_KEY_STR)
	{
		keyType = keyTypeKey;
	}

	else	
	{
		returnCode = RETURN_CODE_FAIL;
	}

	return returnCode;					
}

/*---------------------------------------------------------------------------*/
int mod_synthesizer_ReadTriggerType(json_object *instrumentObject, TriggerType &triggerType)
{
	String triggerTypeStr;

	int returnCode = lib_json_ReadJsonString(triggerTypeStr, 
											 MOD_SYNTHESIZER_NAME_TRIGGER_TYPE_STR,
											 instrumentObject);

	if (triggerTypeStr == MOD_SYNTHESIZER_VALUE_AUTO_STR)
	{
		triggerType = triggerTypeAuto;
	}

	else if (triggerTypeStr == MOD_SYNTHESIZER_VALUE_TRIGGER_STR)
	{
		triggerType = triggerTypeTrigger;
	}

	else if (triggerTypeStr == MOD_SYNTHESIZER_VALUE_REPEATING_STR)
	{
		triggerType = triggerTypeRepeating;
	}

	else	
	{
		returnCode = RETURN_CODE_FAIL;
	}

	return returnCode;					
}

/*---------------------------------------------------------------------------*/
int mod_synthesizer_ReadInstrument(json_object *instrumentObject, Instrument &instrument)
{
	int returnCode = RETURN_CODE_FAIL;

	if ((instrumentObject == NULL) || json_object_object_length(instrumentObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}
				
	returnCode = lib_json_ReadJsonString(instrument.name, 
										 MOD_SYNTHESIZER_NAME_NAME_STR,
										 instrumentObject);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = lib_json_ReadJsonString(instrument.soundFont, 
											 MOD_SYNTHESIZER_NAME_SOUND_FONT_STR,
											 instrumentObject);
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = lib_json_ReadJsonBoolean(instrument.poly, 
											  MOD_SYNTHESIZER_NAME_POLYPHONE_STR,
											  instrumentObject);
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_synthesizer_ReadKeyType(instrumentObject, instrument.keyType); 
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_synthesizer_ReadTriggerType(instrumentObject, instrument.triggerType);					 
	}

	if (returnCode == RETURN_CODE_OK)
	{
		if (instrument.keyType == keyTypePitch)
		{
			returnCode = lib_json_ReadJsonInteger(instrument.pitchStart, 
											  	  MOD_SYNTHESIZER_NAME_PITCH_START_STR,
											  	  instrumentObject);
		} 
		
		else if (instrument.keyType == keyTypeNotes)
		{
			returnCode = lib_json_ReadJsonString(instrument.noteStart, 
											  	 MOD_SYNTHESIZER_NAME_NOTE_START_STR,
											  	 instrumentObject);
		}			 

		else if (instrument.keyType == keyTypeKey)
		{
			returnCode = lib_json_ReadJsonString(instrument.musicKey, 
											  	 MOD_SYNTHESIZER_NAME_KEY_STR,
											  	 instrumentObject);

			if (returnCode == RETURN_CODE_OK)
			{	
				returnCode = lib_json_ReadJsonString(instrument.noteStart, 
											  	  	 MOD_SYNTHESIZER_NAME_NOTE_START_STR,
											  	  	 instrumentObject);
			}									
		}			 			 
	}

	if (returnCode == RETURN_CODE_OK)
	{
		if (instrument.triggerType == triggerTypeRepeating)
		{
			returnCode = lib_json_ReadJsonInteger(instrument.bpmMin, 
											  	  MOD_SYNTHESIZER_NAME_BPM_MIN_STR,
											  	  instrumentObject);

			if (returnCode == RETURN_CODE_OK)
			{	
				returnCode = lib_json_ReadJsonInteger(instrument.bpmMax, 
											  	  	  MOD_SYNTHESIZER_NAME_BPM_MAX_STR,
											  	  	  instrumentObject);
			}

			if (returnCode == RETURN_CODE_OK)
			{	
				returnCode = lib_json_ReadJsonInteger(instrument.bpmStart, 
											  	  	  MOD_SYNTHESIZER_NAME_BPM_START_STR,
											  	  	  instrumentObject);
			}									
		}
	}

	if (returnCode == RETURN_CODE_OK)
	{
		instrument.enabled = true;

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "Succesfully read synth settings of \"%s\"", instrument.name.c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	else
	{
		mod_synthesizer_InitializeInstrument(instrument);

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "Failed to read synth settings of \"%s\"", instrument.name.c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_synthesizer_ReadSynthSettings(const String &filePath)
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
													 		 MOD_SYNTHESIZER_NAME_INSTRUMENTS_STR);

			if (listObject != NULL)
			{
				if (json_object_get_type(listObject) == json_type_array)
				{
					int instrumentAmount = json_object_array_length(listObject);

					if (instrumentAmount <= MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX)
					{
						for (int index = 0; index < instrumentAmount; index++)
						{
							json_object *instrumentObject = json_object_array_get_idx(listObject, index);

							if ((instrumentObject != NULL) && (json_object_object_length(instrumentObject) > 0))
							{
								returnCode = mod_synthesizer_ReadInstrument(instrumentObject, 
																			synthesizerWorkers.instrument[index]);
							
								if (returnCode != RETURN_CODE_OK)
								{
									char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
									sprintf(buffer, 
											"Error: failed to read instrument settings at index \'%d\'", 
											index);
									debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
								}
							}
						}
					}

					else
					{
						debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
													"Error: to many instruments defined in synth settings");

						returnCode = RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
					}
				}						
			}
		}

		if (returnCode == RETURN_CODE_OK)
		{
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
										"Succesfully read the synth settings file");
		}

		else
		{
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
										"Error: failed reading synth setings file");
		}
	}

	else
	{
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
									"Error: sensor settings file does not exist");
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Synthesizer functions
 *
 *---------------------------------------------------------------------------*/
void mod_synthesizer_InitializeInstrument(Instrument instrument)
{
	instrument.enabled		= false;
	instrument.synth 		= NULL;

	instrument.poly			= false;

	instrument.keyType		= keyTypeUnknown;
	instrument.triggerType	= triggerTypeUnknown;

	instrument.pitchStart	= 0;
	instrument.bpmMin		= 0;
	instrument.bpmMax		= 0;
	instrument.bpmStart		= 0;

	instrument.currentNote	= 0;
	instrument.currentPitch	= 8192;
	instrument.currentBpm	= 0;
	
	instrument.bpmTimer		= 0;
	instrument.bpmTimerEnd	= 0;

	instrument.velocity		= MOD_SYNTHESIZER_VELOCITY_MAX;

	instrument.name.clear();
	instrument.soundFont.clear();
	instrument.noteStart.clear();
	instrument.musicKey.clear();
}

/*---------------------------------------------------------------------------*/
int mod_synthesizer_InitializeFluidSynthSettings()
{
	int returnCode = RETURN_CODE_OK;

	synthesizerWorkers.settings = new_fluid_settings();
	if (synthesizerWorkers.settings == NULL)
	{
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, 
								   "Failed to create the synthesizer settings");
		returnCode = RETURN_CODE_FAIL;
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_synthesizer_InitializeFluidSynthDriver()
{
	int returnCode = RETURN_CODE_OK;

	fluid_settings_setstr(synthesizerWorkers.settings, 
						  MOD_SYNTHESIZER_AUDIO_DRIVER, 
						  MOD_SYNTHESIZER_AUDIO_DRIVER_ALSA);

	for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
	{
		if (synthesizerWorkers.instrument[index].synth)
		{
			synthesizerWorkers.adriver = new_fluid_audio_driver(synthesizerWorkers.settings, 
																synthesizerWorkers.instrument[index].synth);
			if (synthesizerWorkers.adriver == NULL)
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, 
						"Failed to create audio driver for \"%s\"", 
						synthesizerWorkers.instrument[index].name.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_synthesizer_CreateInstrument(Instrument &instrument)
{
	int returnCode = RETURN_CODE_OK;

	instrument.synth = new_fluid_synth(synthesizerWorkers.settings);
	if (instrument.synth == NULL)
	{
		returnCode = RETURN_CODE_FAIL;

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "Failed to create synth \"%s\"", instrument.name.c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
		
	}

	if (returnCode == RETURN_CODE_OK)
	{
		String filePath = synthesizerSettings.soundFontsDirPath + instrument.soundFont;

		returnCode = lib_file_CheckFile(filePath);

		if (returnCode == RETURN_CODE_OK)
		{
			if (fluid_synth_sfload(instrument.synth, filePath.c_str(), 1) == -1)
			{
				returnCode = RETURN_CODE_FAIL;

				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "Failed to load sound font \"%s\"", instrument.soundFont.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
			}
		}

		else
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "Failed: sound font \"%s\" does not exist", instrument.soundFont.c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
		}
	}

	if (returnCode == RETURN_CODE_OK)
	{
		instrument.currentNote 	= 64;
		instrument.currentPitch	= instrument.pitchStart;
		instrument.currentBpm	= instrument.bpmStart;
		
		if(instrument.triggerType == triggerTypeAuto)
		{
			fluid_synth_noteon(instrument.synth, 0, instrument.currentNote, instrument.velocity);
		}

		else if ((instrument.triggerType == triggerTypeRepeating) &&
				 (instrument.currentBpm > 0))
		{
			float bps = (100 / (instrument.currentBpm / 60.0));
			instrument.bpmTimerEnd = (int)bps;
		}
	}

	if (returnCode == RETURN_CODE_OK)
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "Succesfully created synth \"%s\"", instrument.name.c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	else
	{
		mod_synthesizer_InitializeInstrument(instrument);

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "Failed to create synth \"%s\"", instrument.name.c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
	}

	return returnCode;
}


/*---------------------------------------------------------------------------*/
void mod_synthesizer_PlayNote(Instrument &instrument, const int &note)
{
	if (!instrument.poly)
	{
		fluid_synth_noteoff(instrument.synth, 
							0, 
							instrument.currentNote);
	}

	fluid_synth_noteon(instrument.synth, 
					   0, 
					   note,
					   instrument.velocity);

	instrument.currentNote = note;
}

/*---------------------------------------------------------------------------*/
void mod_synthesizer_InstrumentPitch(const int &instrument, const int &pitchAmount)
{
	int returnCode = RETURN_CODE_OK;

	if (instrument > MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	if ((returnCode == RETURN_CODE_OK) && (synthesizerWorkers.synthEnabled))
	{
		if (synthesizerWorkers.instrument[instrument].enabled)
		{
			if (synthesizerWorkers.instrument[instrument].keyType == keyTypePitch)
			{
				unsigned int pitchChange = synthesizerWorkers.instrument[instrument].currentPitch + 
											(pitchAmount * MOD_SYNTHESIZER_PITCH_SENSITIVITY);

				if (pitchChange <= MOD_SYNTHESIZER_PITCH_BEND_MAX)
				{
					synthesizerWorkers.instrument[instrument].currentPitch = pitchChange;
					fluid_synth_pitch_bend(synthesizerWorkers.instrument[instrument].synth, 
										   0, 
										   synthesizerWorkers.instrument[instrument].currentPitch);
				}
			}

			if (synthesizerWorkers.instrument[instrument].keyType == keyTypeNotes)
			{
				int noteChange = synthesizerWorkers.instrument[instrument].currentNote + pitchAmount;
				if (noteChange <= MOD_SYNTHESIZER_NOTE_MAX)
				{
					
					if (synthesizerWorkers.instrument[instrument].triggerType == triggerTypeAuto)
					{
						mod_synthesizer_PlayNote(synthesizerWorkers.instrument[instrument],
												 noteChange);
					}

					else if (synthesizerWorkers.instrument[instrument].triggerType == triggerTypeRepeating)
					{
						synthesizerWorkers.instrument[instrument].currentNote = noteChange;
					}
				}
			}
		}
	}	
}

/*---------------------------------------------------------------------------*/
void mod_synthesizer_InstrumentTrigger(const int &instrument)
{
	int returnCode = RETURN_CODE_OK;

	if (instrument > MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	if ((returnCode == RETURN_CODE_OK) && (synthesizerWorkers.synthEnabled))
	{
		if ((synthesizerWorkers.instrument[instrument].enabled) &&
			(synthesizerWorkers.instrument[instrument].triggerType == triggerTypeTrigger))
		{
			mod_synthesizer_PlayNote(synthesizerWorkers.instrument[instrument],
									 synthesizerWorkers.instrument[instrument].currentNote);
		}
	}	
}

/*---------------------------------------------------------------------------*/
void mod_synthesizer_InstrumentSetBpm(const int &instrument, const int &bpmChange)
{
	int returnCode = RETURN_CODE_OK;

	if (instrument > MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	if ((returnCode == RETURN_CODE_OK) && (synthesizerWorkers.synthEnabled))
	{
		if ((synthesizerWorkers.instrument[instrument].enabled) &&
			(synthesizerWorkers.instrument[instrument].triggerType == triggerTypeRepeating))
		{	
			int bpm = synthesizerWorkers.instrument[instrument].currentBpm + (bpmChange * 8);
			if ((bpm >= synthesizerWorkers.instrument[instrument].bpmMin) &&
				(bpm <= synthesizerWorkers.instrument[instrument].bpmMax))
			{
				synthesizerWorkers.instrument[instrument].currentBpm = bpm;

				float bps = (100 / (synthesizerWorkers.instrument[instrument].currentBpm / 60.0));
				synthesizerWorkers.instrument[instrument].bpmTimerEnd = (int)bps;
			}
		}
	}
}

/*---------------------------------------------------------------------------*/
void mod_synthesizer_InstrumentSetVelocity(const int &instrument, const int &velocityChange)
{
	int returnCode = RETURN_CODE_OK;

	if (instrument > MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	if ((returnCode == RETURN_CODE_OK) && (synthesizerWorkers.synthEnabled))
	{
		if (synthesizerWorkers.instrument[instrument].enabled)
		{
			int velocity = synthesizerWorkers.instrument[instrument].velocity + (velocityChange * 8);

			if (velocity <= MOD_SYNTHESIZER_VELOCITY_MAX)
			{
				synthesizerWorkers.instrument[instrument].velocity += velocity;
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * Synthesizer timer functions
 *
 *---------------------------------------------------------------------------*/
void mod_synthesizer_Timer100HzEvent(int)
{
	synthesizerWorkers.timer1HzTrigger = true;
}

/*---------------------------------------------------------------------------*/
void mod_synthesizer_Timer100HzHandler()
{
	if (synthesizerWorkers.timer1HzTrigger)
	{
		synthesizerWorkers.timer1HzTrigger = false;
		
		for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
		{
			if ((synthesizerWorkers.instrument[index].triggerType == triggerTypeRepeating) &&
				(synthesizerWorkers.instrument[index].enabled) &&
				(synthesizerWorkers.synthEnabled))
			{
				if (++synthesizerWorkers.instrument[index].bpmTimer >= 
					synthesizerWorkers.instrument[index].bpmTimerEnd)
				{
					synthesizerWorkers.instrument[index].bpmTimer = 0;

					fluid_synth_all_notes_off(synthesizerWorkers.instrument[index].synth, 0);

					fluid_synth_noteon(synthesizerWorkers.instrument[index].synth, 
									   0, 
									   synthesizerWorkers.instrument[index].currentNote, 
									   MOD_SYNTHESIZER_VELOCITY_MAX);
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 * MQTT Client message actions
 *
 *---------------------------------------------------------------------------*/
int	mod_synthesizer_MessageDiscover(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "received on topic \"%s\"", mqttMessage.messageTopic.c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);

	json_object *jsonObject = json_object_new_object();

	returnCode = lib_file_CheckFile(synthesizerSettings.settingsPath);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = lib_json_ReadJsonFile(&jsonObject, synthesizerSettings.settingsPath);

		if (returnCode == RETURN_CODE_OK)
		{
			String jsonData = json_object_to_json_string(jsonObject);

			if (jsonData.length() > 0)
			{
				lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_AUDIO_SETTINGS, jsonData.c_str());

				if (returnCode == RETURN_CODE_OK)
				{
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
												"Succesfully send synth settings");
				}
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_synthesizer_MessageAudioSettings(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "received on topic \"%s\"", mqttMessage.messageTopic.c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);

	if (mqttMessage.messageData.length() > 0)
	{
		json_object *jsonObject = json_object_new_object();

		jsonObject = json_tokener_parse(mqttMessage.messageData.c_str());

		if ((jsonObject != NULL) && (json_object_object_length(jsonObject) > 0))
		{
			returnCode = lib_json_WriteJsonFile(synthesizerSettings.settingsPath, jsonObject);

			if (returnCode == RETURN_CODE_OK)
			{
				returnCode = RETURN_CODE_APPLICATION_CLEAN_EXIT;

				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, 
										   "Succesfully received synth settings");
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_synthesizer_MessagePlayPause(MQTTMessage &mqttMessage)
{
	int returnCode = RETURN_CODE_OK;

	char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
	sprintf(buffer, "received on topic \"%s\"", mqttMessage.messageTopic.c_str());
	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);

	if (mqttMessage.messageData == "play")
	{
		synthesizerWorkers.synthEnabled = true;
	}

	else if (mqttMessage.messageData == "pause")
	{
		synthesizerWorkers.synthEnabled = false;

		for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
		{
			fluid_synth_all_notes_off(synthesizerWorkers.instrument[index].synth, 0);
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client message action table for MQTT messages
 *
 *---------------------------------------------------------------------------*/
const MQTTMessageAction mod_mqtt_client_MQTTActionTable[] =
{
	{	LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCOVER	,	mod_synthesizer_MessageDiscover			},
	{	LIB_MQTT_TOPIC_RESOURCE_AUDIO_SETTINGS			,	mod_synthesizer_MessageAudioSettings	},
	{	LIB_MQTT_TOPIC_RESOURCE_PLAY_PAUSE				,	mod_synthesizer_MessagePlayPause		}
};

/*-----------------------------------------------------------------------------
 * MQTT Client Action register
 *
 *---------------------------------------------------------------------------*/
int mod_synthesizer_RegisterMQTTActions()
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
 * Synthesizer jump table is used for inifile settings and control message commands
 *
 *---------------------------------------------------------------------------*/
const SettingsObjectTableObject mod_synthesizer_SettingsTable[] =
{
	{	MOD_SYNTHESIZER_SETTING_SET_SETTINGS_PATH_STR	,	mod_synthesizer_SetSettingsPath			},
	{	MOD_SYNTHESIZER_SETTING_SF_DIR_PATH_STR			,	mod_synthesizer_SetSoundFontsDirPath	}
};

/*-----------------------------------------------------------------------------
 * Synthesizer settings handler
 *
 *---------------------------------------------------------------------------*/
int mod_synthesizer_SettingsHandler(SettingsObjectDataList &settingsObjectDataList)
{
	int returnCode = RETURN_CODE_OK;

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting settings handler");

	size_t 	index, 
			sizeofJumpTableObjects = (sizeof(mod_synthesizer_SettingsTable) /
									  sizeof(mod_synthesizer_SettingsTable[0]));

	for (SettingsObjectDataList::const_iterator settingsObjectDataIt = settingsObjectDataList.begin();
		 settingsObjectDataIt != settingsObjectDataList.end(); settingsObjectDataIt++)
	{
		for (index = 0; index < sizeofJumpTableObjects; index++)
		{
			if ((settingsObjectDataIt->settingsName == mod_synthesizer_SettingsTable[index].settingsName) &&
				(mod_synthesizer_SettingsTable[index].settingsSetter != 0))
			{
				SettingsObjectData settingsObjectData = *settingsObjectDataIt;

				(mod_synthesizer_SettingsTable[index].settingsSetter)(settingsObjectData);
			}
		}
	}

	return returnCode;
}
/*-----------------------------------------------------------------------------
 * Synthesizer pre initialisation
 *
 *---------------------------------------------------------------------------*/
void mod_synthesizer_PreInitialisation()
{
    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting pre-initialization");
	/* Initialise the settings */
	synthesizerSettings.settingsPath 		= MOD_SYNTHESIZER_INITIAL_SETTINGS_PATH_STR;
	synthesizerSettings.soundFontsDirPath 	= MOD_SYNTHESIZER_INITIAL_SF_DIR_PATH_STR;

	/* Initialise the workers */
	synthesizerWorkers.timer1HzId 		= 0;
	synthesizerWorkers.timer1HzTrigger 	= false;

	synthesizerWorkers.synthEnabled		= true;

	synthesizerWorkers.settings 		= NULL;
	synthesizerWorkers.synth 			= NULL;
	synthesizerWorkers.adriver 			= NULL;

	for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
	{
		mod_synthesizer_InitializeInstrument(synthesizerWorkers.instrument[index]);
	}

	for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
	{
		synthesizerWorkers.instrument[index].velocity = MOD_SYNTHESIZER_VELOCITY_MAX;
	}
}

/*-----------------------------------------------------------------------------
 * Synthesizer post initialisation, right before entering mainloop
 *
 *
 *---------------------------------------------------------------------------*/
int mod_synthesizer_PostInitialisation()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting post-initialization");

		/* Finalize module specific initialisation before mainloop here */
		if (!lib_time_SetupAndStartTimer(LIB_TIME_ID_MOD_SYNTHESIZER_100HZ,
										 LIB_TIME_TIMER_100HZ_VALUE,
										 (void*)mod_synthesizer_Timer100HzEvent,
										 &synthesizerWorkers.timer1HzId))
		{
			returnCode = RETURN_CODE_FAIL;
		}

		mod_synthesizer_RegisterMQTTActions();

		returnCode = mod_synthesizer_ReadSynthSettings(synthesizerSettings.settingsPath);

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_synthesizer_InitializeFluidSynthSettings();
		}

		if (returnCode == RETURN_CODE_OK)
		{
			for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
			{
				if (synthesizerWorkers.instrument[index].enabled)
				{
					mod_synthesizer_CreateInstrument(synthesizerWorkers.instrument[index]);
				}
			}
		}

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_synthesizer_InitializeFluidSynthDriver();
		}

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*-----------------------------------------------------------------------------
 * Synthesizer module cleanup and destruction
 *
 *---------------------------------------------------------------------------*/
int mod_synthesizer_DestroyData()
{
    int returnCode = RETURN_CODE_OK;

    debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "starting destroy data");

	/* Clean module specific data here */
	if (synthesizerWorkers.adriver)
	{
		delete_fluid_audio_driver(synthesizerWorkers.adriver);
	}

	if (synthesizerWorkers.settings)
	{
		delete_fluid_settings(synthesizerWorkers.settings);
	}

	for (int index = 0; index < MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX; index++)
	{
		if (synthesizerWorkers.instrument[index].synth)
		{
			delete_fluid_synth(synthesizerWorkers.instrument[index].synth);
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Synthesizer mainloop functionality
 *
 *---------------------------------------------------------------------------*/
int mod_synthesizer_MainloopFunction()
{
	try
	{
		int returnCode = RETURN_CODE_OK;

        mod_synthesizer_Timer100HzHandler();

		return returnCode;
	}
	catch (const std::exception &ex)
	{
		return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
	}
}

/*---------------------------------------------------------------------------*/