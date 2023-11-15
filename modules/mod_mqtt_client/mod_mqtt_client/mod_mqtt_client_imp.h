/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MQTT_CLIENT_IMP_H
#define MOD_MQTT_CLIENT_IMP_H

/*-----------------------------------------------------------------------------
 * MQTT Client include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_mqtt_client_defines.h"

/*-----------------------------------------------------------------------------
 * MQTT Client define values
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * MQTT Client ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR_LENGTH_MIN        1
#define MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR_LENGTH_MAX        32

/*---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_SERVER_ADDRESS_STR_LENGTH_MIN   7
#define MOD_MQTT_CLIENT_SETTING_SERVER_ADDRESS_STR_LENGTH_MAX   15

/*---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_SERVER_PORT_MIN                 1
#define MOD_MQTT_CLIENT_SETTING_SERVER_PORT_MAX                 65535

/*---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_CREDENTIALS_LENGTH_MIN          1
#define MOD_MQTT_CLIENT_SETTING_CREDENTIALS_LENGTH_MAX          128

/*---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_RECONNECT_DELAY_MIN             0
#define MOD_MQTT_CLIENT_SETTING_RECONNECT_DELAY_MAX             60

/*---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_LOG_MODE_STR_LENGTH_MIN         3
#define MOD_MQTT_CLIENT_SETTING_LOG_MODE_STR_LENGTH_MAX         7

/*-----------------------------------------------------------------------------
 * MQTT Client initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_INITIAL_CLIENT_ID_STR                   "jmp-device-00"
#define MOD_MQTT_CLIENT_INITIAL_SERVER_ID_STR                   "jmp-device-server"
#define MOD_MQTT_CLIENT_INITIAL_SERVER_ADDRESS_STR              "192.168.0.10"
#define MOD_MQTT_CLIENT_INITIAL_SERVER_PORT                     1883
#define MOD_MQTT_CLIENT_INITIAL_SERVER_LOGIN_STR                "jmserver"
#define MOD_MQTT_CLIENT_INITIAL_SERVER_PSWRD_STR                ""
#define MOD_MQTT_CLIENT_INITIAL_RECONNECT_DELAY                 20
#define MOD_MQTT_CLIENT_INITIAL_LOG_MODE_STR                    LIB_MQTT_LOG_MODE_NONE_STR

/*-----------------------------------------------------------------------------
 * MQTT Client settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_MQTT_CLIENT_SETTING_CLIENT_ID_STR                   "mqtt-client-id"
#define MOD_MQTT_CLIENT_SETTING_SERVER_ID_STR                   "mqtt-server-id"
#define MOD_MQTT_CLIENT_SETTING_SERVER_ADDRESS_STR              "mqtt-client-server-address"
#define MOD_MQTT_CLIENT_SETTING_SERVER_PORT_STR                 "mqtt-client-server-port"
#define MOD_MQTT_CLIENT_SETTING_SERVER_LOGIN_STR                "mqtt-client-server-username"
#define MOD_MQTT_CLIENT_SETTING_SERVER_PSWRD_STR                "mqtt-client-server-password"
#define MOD_MQTT_CLIENT_SETTING_RECONNECT_DELAY_STR             "mqtt-client-reconnect-delay"
#define MOD_MQTT_CLIENT_SETTING_LOG_MODE_STR                    "mqtt-client-log-mode"

/*-----------------------------------------------------------------------------
 * MQTT Client settings
 *
 *---------------------------------------------------------------------------*/
typedef struct MQTTClientSettings
{
    String      clientID;
    String      serverID;
    String      serverAddress;
    uint16_t    serverPort;
    String      serverUsername;
    String      serverPassword;

    int         reconnectDelay;
    String      logMode;

} MQTTClientSettings;

/*-----------------------------------------------------------------------------
 * MQTT Client workers
 *
 *---------------------------------------------------------------------------*/
typedef struct MQTTClientWorkers
{
    class MQTTClient*       mqttClient;
    MQTTMessageList         mqttMessageList;
    MQTTMessageActionList   mqttMessageActionList;

    timer_t                 timer1HzId;
    bool                    timer1HzTrigger;

    int                     reconnectTimer;

} MQTTClientWorkers;

/*-----------------------------------------------------------------------------
 * MQTT Client exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_mqtt_client_PreInitialisation();
int  mod_mqtt_client_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_mqtt_client_PostInitialisation();
int  mod_mqtt_client_DestroyData();
int  mod_mqtt_client_MainloopFunction();
int  mod_mqtt_client_TestFunction();

/*---------------------------------------------------------------------------*/
#endif /* MOD_MQTT_CLIENT_IMP_H											     */
/*---------------------------------------------------------------------------*/
