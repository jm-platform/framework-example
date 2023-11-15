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
#include "mod_mqtt_client.h"

/*---------------------------------------------------------------------------*/
#include "mod_mqtt_client_imp.h"

/*-----------------------------------------------------------------------------
 * MQTT Client settings and workers
 *
 *---------------------------------------------------------------------------*/
extern MQTTClientSettings mqttClientSettings;
extern MQTTClientWorkers  mqttClientWorkers;

/*-----------------------------------------------------------------------------
 * MQTT Client setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetClientId(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR_LENGTH_MAX))
	{
		mqttClientSettings.clientID = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetServerId(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR_LENGTH_MAX))
	{
		mqttClientSettings.serverID = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetServerAdress(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MQTT_CLIENT_SETTING_SERVER_ADDRESS_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MQTT_CLIENT_SETTING_SERVER_ADDRESS_STR_LENGTH_MAX))
	{
		mqttClientSettings.serverAddress = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetServerPort(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		uint16_t serverPort = stoi(settingsObjectData.settingsValue[0]);

		if (serverPort >= MOD_MQTT_CLIENT_SETTING_SERVER_PORT_MIN)
		{
			mqttClientSettings.serverPort = serverPort;

			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
										MOD_DEBUG_MESSAGE_EQUAL_STR, 
										settingsObjectData.settingsValue[0].c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetServerUsername(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MQTT_CLIENT_SETTING_CREDENTIALS_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MQTT_CLIENT_SETTING_CREDENTIALS_LENGTH_MAX))
	{
		mqttClientSettings.serverUsername = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetServerPassword(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MQTT_CLIENT_SETTING_CREDENTIALS_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MQTT_CLIENT_SETTING_CREDENTIALS_LENGTH_MAX))
	{
		mqttClientSettings.serverPassword = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetReconnectDelay(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if(settingsObjectData.settingsCount == 1)
	{
		uint16_t delay = stoi(settingsObjectData.settingsValue[0]);

		if (delay <= MOD_MQTT_CLIENT_SETTING_RECONNECT_DELAY_MAX)
		{
			mqttClientSettings.reconnectDelay = delay;

			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
										MOD_DEBUG_MESSAGE_EQUAL_STR, 
										settingsObjectData.settingsValue[0].c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetLogMode(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() >= MOD_MQTT_CLIENT_SETTING_LOG_MODE_STR_LENGTH_MIN) &&
	   (settingsObjectData.settingsValue[0].length() <= MOD_MQTT_CLIENT_SETTING_LOG_MODE_STR_LENGTH_MAX))
	{
		mqttClientSettings.logMode = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * MQTT Client module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_mqtt_client_ModuleInterface =
{

		/* application info */
        MOD_MQTT_CLIENT_NAME,
		MOD_MQTT_CLIENT_VERSION,
		MOD_MQTT_CLIENT_BUILD,

		/* Exposed functions of the module */
		mod_mqtt_client_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_mqtt_client_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_mqtt_client_PostInitialisation,			    /* Module PostInitialisationPtr */
		mod_mqtt_client_DestroyData,					/* Module DestroyDataPtr */
		mod_mqtt_client_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    			/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/