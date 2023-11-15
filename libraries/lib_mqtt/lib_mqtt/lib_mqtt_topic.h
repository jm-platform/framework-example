/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_MQTT_TOPIC_H
#define LIB_MQTT_TOPIC_H

/*-----------------------------------------------------------------------------
 * Lib mqtt topic include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include "../../lib_defines/lib_defines.h"

/*-----------------------------------------------------------------------------
 * Lib mqtt topic define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_DEST_WEB_API_STR                     "jmp-web-api"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_UNKNOWN_STR                          "unknown"
#define LIB_MQTT_TOPIC_INVALID_STR                          "invalid"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_SERVER_STR                           "server"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_SEPERATOR_STR                        "/"
#define LIB_MQTT_TOPIC_WILDCARD_STR                         "/#"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_MESSAGE_DATA_ALL_STR                       "all"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_DEVICE_INFO_STORAGE_STR                    "storage-info"
#define LIB_MQTT_DEVICE_INFO_SYSTEM_STR                     "system-info"
#define LIB_MQTT_DEVICE_INFO_PLATFORM_STR                   "platform-info"

/*-----------------------------------------------------------------------------
 * Lib mqtt topic definitions
 *
 *---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_CONNECTIONS_STR                      "connections"

#define LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_REGISTER        1001
#define LIB_MQTT_TOPIC_CONNECTIONS_REGISTER_STR             "register"

#define LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_CONNECT         1002
#define LIB_MQTT_TOPIC_CONNECTIONS_CONNECT_STR              "connect"

#define LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCONNECT      1003
#define LIB_MQTT_TOPIC_CONNECTIONS_DISCONNECT_STR           "disconnect"

#define LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCOVER        1004
#define LIB_MQTT_TOPIC_CONNECTIONS_DISCOVER_STR             "discover"

#define LIB_MQTT_TOPIC_RESOURCE_NOTIFICATION                1005
#define LIB_MQTT_TOPIC_NOTIFICATION_STR                     "notification"

#define LIB_MQTT_TOPIC_RESOURCE_DEBUGGING                   1006
#define LIB_MQTT_TOPIC_DEBUGGING_STR                        "debugging"

#define LIB_MQTT_TOPIC_RESOURCE_LOG                         1007
#define LIB_MQTT_TOPIC_LOG_STR                              "log"

#define LIB_MQTT_TOPIC_RESOURCE_DATA                        1008
#define LIB_MQTT_TOPIC_DATA_STR                             "data"

#define LIB_MQTT_TOPIC_RESOURCE_VALUE                        1009
#define LIB_MQTT_TOPIC_VALUE_STR                            "value"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_RESOURCE_GENERAL                     1
#define LIB_MQTT_TOPIC_GENERAL_STR                          "general"

#define LIB_MQTT_TOPIC_RESOURCE_DEVICE_INFO                 2
#define LIB_MQTT_TOPIC_DEVICE_INFO_STR                      "device-info"

#define LIB_MQTT_TOPIC_RESOURCE_JMP_VERSION                 3
#define LIB_MQTT_TOPIC_JMP_VERSION_STR                      "jmp-version"

#define LIB_MQTT_TOPIC_RESOURCE_MQTT_VERSION                4
#define LIB_MQTT_TOPIC_MQTT_VERSION_STR                     "mqtt-version"

#define LIB_MQTT_TOPIC_RESOURCE_SKELETON_TEST               5
#define LIB_MQTT_TOPIC_SKELETON_TEST_STR                    "skeleton-test"

#define LIB_MQTT_TOPIC_RESOURCE_TOGGLE_DATA                 6
#define LIB_MQTT_TOPIC_TOGGLE_DATA_STR                      "toggle-data"

#define LIB_MQTT_TOPIC_RESOURCE_SET_DATA                    7
#define LIB_MQTT_TOPIC_SET_DATA_STR                         "set-data"

#define LIB_MQTT_TOPIC_RESOURCE_TOGGLE_OUTPUT               8
#define LIB_MQTT_TOPIC_TOGGLE_OUTPUT_STR                    "toggle-output"

#define LIB_MQTT_TOPIC_RESOURCE_SET_OUTPUT                  9
#define LIB_MQTT_TOPIC_SET_OUTPUT_STR                       "set-output"

#define LIB_MQTT_TOPIC_RESOURCE_DIGITAL_INPUT               10
#define LIB_MQTT_TOPIC_DIGITAL_INPUT_STR                    "digital-input"

#define LIB_MQTT_TOPIC_RESOURCE_LOCATION                    11
#define LIB_MQTT_TOPIC_LOCATION_STR                         "location"

#define LIB_MQTT_TOPIC_RESOURCE_ALARM                       12
#define LIB_MQTT_TOPIC_ALARM_STR                            "alarm"

#define LIB_MQTT_TOPIC_RESOURCE_LOGIN_USER                  13
#define LIB_MQTT_TOPIC_LOGIN_USER_STR                       "login-user"

#define LIB_MQTT_TOPIC_RESOURCE_CREATE_USER                 14
#define LIB_MQTT_TOPIC_CREATE_USER_STR                      "create-user"

#define LIB_MQTT_TOPIC_RESOURCE_SENSOR_SETTINGS             15
#define LIB_MQTT_TOPIC_SENSOR_SETTINGS_STR            		"sensor-settings"

#define LIB_MQTT_TOPIC_RESOURCE_PUPPET_DEVICES              16
#define LIB_MQTT_TOPIC_PUPPET_DEVICES_STR           		"puppet-devices"

#define LIB_MQTT_TOPIC_RESOURCE_AUDIO_SETTINGS              17
#define LIB_MQTT_TOPIC_AUDIO_SETTINGS_STR           		"audio-settings"

#define LIB_MQTT_TOPIC_RESOURCE_PLAY_PAUSE                  18
#define LIB_MQTT_TOPIC_PLAY_PAUSE_STR           		    "play-pause"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_RESOURCE_ACCEL_X                     21
#define LIB_MQTT_TOPIC_ACCEL_X_STR           		        "accelerometer-x"

#define LIB_MQTT_TOPIC_RESOURCE_ACCEL_Y                     22
#define LIB_MQTT_TOPIC_ACCEL_Y_STR           		        "accelerometer-Y"

#define LIB_MQTT_TOPIC_RESOURCE_ACCEL_Z                     23
#define LIB_MQTT_TOPIC_ACCEL_Z_STR           		        "accelerometer-Z"

#define LIB_MQTT_TOPIC_RESOURCE_GYRO_X                      24
#define LIB_MQTT_TOPIC_GYRO_X_STR           		        "gyroscope-x"

#define LIB_MQTT_TOPIC_RESOURCE_GYRO_Y                      25
#define LIB_MQTT_TOPIC_GYRO_Y_STR           		        "gyroscope-y"

#define LIB_MQTT_TOPIC_RESOURCE_GYRO_Z                      26
#define LIB_MQTT_TOPIC_GYRO_Z_STR           		        "gyroscope-z"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_TOPIC_RESOURCE_SUB_DEVICE_ONLINE           27
#define LIB_MQTT_TOPIC_SUB_DEVICE_ONLINE_STR                "sub-device-online"

#define LIB_MQTT_TOPIC_RESOURCE_SUB_DEVICE_SENSORS          28
#define LIB_MQTT_TOPIC_SUB_DEVICE_SENSORS_STR               "sub-device-sensors"

#define LIB_MQTT_TOPIC_RESOURCE_SUB_DEVICE_STATUS           29
#define LIB_MQTT_TOPIC_SUB_DEVICE_STATUS_STR                "sub-device-status"

/*-----------------------------------------------------------------------------
 * Lib mqtt topic exposed functions
 *
 *---------------------------------------------------------------------------*/
int lib_mqtt_tokenizeTopic(const char* topic, 
                           String &dest, 
                           String &source, 
                           int &topicID, 
                           String& subTopic);
String lib_mqtt_ConvertTopicId(const int topicID);
String lib_mqtt_CreateTopic(const String source, const String dest, const int topicID);
String lib_mqtt_CreateTopic(const String source, 
                            const String dest, 
                            const int topicID, 
                            const String subTopic);

/*---------------------------------------------------------------------------*/
#endif /* LIB_MQTT_TOPIC_H											         */
/*---------------------------------------------------------------------------*/