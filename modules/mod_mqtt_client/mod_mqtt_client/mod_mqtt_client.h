/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MQTT_CLIENT_H
#define MOD_MQTT_CLIENT_H

/*-----------------------------------------------------------------------------
 * MQTT Client include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * MQTT Client exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_mqtt_client_SetClientId(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetServerId(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetServerAdress(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetServerPort(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetServerUsername(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetServerPassword(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetReconnectDelay(SettingsObjectData &settingsObjectData);
int	mod_mqtt_client_SetLogMode(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * MQTT Client module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_mqtt_client_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_MQTT_CLIENT_H											         */
/*---------------------------------------------------------------------------*/
