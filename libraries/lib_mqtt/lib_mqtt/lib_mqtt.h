/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_MQTT_H
#define LIB_MQTT_H

/*-----------------------------------------------------------------------------
 * Lib mqtt include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include <mosquittopp.h>

/*---------------------------------------------------------------------------*/
#include "lib_mqtt_topic.h"

/*---------------------------------------------------------------------------*/
#include "../../application_interface/application_interface.h"
#include "../../lib_defines/lib_defines.h"

/*---------------------------------------------------------------------------*/
#include "../../../modules/mod_debug/mod_debug.h"

/*-----------------------------------------------------------------------------
 * Lib mqtt define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_MQTT_QOS_AT_MOST_ONCE                           0
#define LIB_MQTT_QOS_AT_LEAST_ONCE                          1
#define LIB_MQTT_QOS_EXACTLY_ONCE                           2

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_LOG_MODE_NONE_STR                          "none"
#define LIB_MQTT_LOG_MODE_ALL_STR                           "all"
#define LIB_MQTT_LOG_MODE_DEBUG_STR                         "debug"
#define LIB_MQTT_LOG_MODE_ERROR_STR                         "error"
#define LIB_MQTT_LOG_MODE_GENERAL_STR                       "general"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_NOTIFICATION_INFO_STR                      "info"
#define LIB_MQTT_NOTIFICATION_NOTICE_STR                    "notice"
#define LIB_MQTT_NOTIFICATION_ALERT_STR                     "alert"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_SYSTEM_TYPE_UNKNOWN                        0
#define LIB_MQTT_SYSTEM_TYPE_CLIENT                         1
#define LIB_MQTT_SYSTEM_TYPE_SERVER                         2

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_ARGUMENT_SEPERATOR_STR                     ':'

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_ACK_STR                                    "ack"
#define LIB_MQTT_NACK_STR                                   "nack"

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_CONNECTIONS_TYPE_CONNECT_STR               LIB_MQTT_TOPIC_CONNECTIONS_CONNECT_STR
#define LIB_MQTT_CONNECTIONS_TYPE_DISCONNECT_STR            LIB_MQTT_TOPIC_CONNECTIONS_DISCONNECT_STR

/*---------------------------------------------------------------------------*/
#define LIB_MQTT_ACTION_ALARM_STATE_UNKNOWN_STR             "unknown"
#define LIB_MQTT_ACTION_ALARM_STATE_DISARMED_STR            "disarmed"
#define LIB_MQTT_ACTION_ALARM_STATE_ARMED_STR               "armed"
#define LIB_MQTT_ACTION_ALARM_STATE_ARM_STR                 "arm"
#define LIB_MQTT_ACTION_ALARM_STATE_DISARM_STR              "disarm"
#define LIB_MQTT_ACTION_ALARM_STATE_TRIGGERED_STR           "triggered"

/*-----------------------------------------------------------------------------
 * Lib mqtt type definitions
 *
 *---------------------------------------------------------------------------*/
typedef struct MQTTMessage
{
    String messageDest;
    String messageSource;
    int    messageTopicID;
    String messageTopic;
    String messageSubTopic;
    String messageData;

} MQTTMessage;
/*---------------------------------------------------------------------------*/
typedef std::list <MQTTMessage> MQTTMessageList;

/*---------------------------------------------------------------------------*/
typedef int MQTTMessageTopicId; 
typedef int MQTTMessageFunction(MQTTMessage&);

/*---------------------------------------------------------------------------*/
typedef struct MQTTMessageAction
{
	MQTTMessageTopicId    	    topicID;
	MQTTMessageFunction         *mqttAction;

} MQTTMessageAction;

/*---------------------------------------------------------------------------*/
typedef std::list <MQTTMessageAction> MQTTMessageActionList;

/*---------------------------------------------------------------------------*/
typedef void Callback;
typedef void MQTTConnector();
typedef int  MQTTSubscriber();
typedef void MQTTDataHandler(MQTTMessage&);
typedef int  MQTTPublisher(MQTTMessage&);
typedef void MQTTActionHandler(const MQTTMessageAction&);

/*---------------------------------------------------------------------------*/
class MQTTClient : public mosqpp::mosquittopp 
{
    Callback on_connect(int returnCode __attribute__((unused)));
    Callback on_disconnect(int returnCode __attribute__((unused)));    
    Callback on_message(const struct mosquitto_message *message);
    Callback on_log(int level, const char* logMessage);

    MQTTConnector       *connector;
    MQTTSubscriber      *subscriber;
    MQTTDataHandler     *dataHandler;
    MQTTConnector       *disconnector;

    public:
        void registerCallbackFunctions(MQTTConnector *connector,
                                       MQTTSubscriber *subscriber,
                                       MQTTDataHandler *dataHandler,
                                       MQTTConnector *disconnector);

        bool    connected;
        String  logMode; 

};

/*-----------------------------------------------------------------------------
 * Lib mqtt settings
 *
 *---------------------------------------------------------------------------*/
typedef struct MQTTSettings
{
    MQTTPublisher       *mqttPublisher;
    MQTTActionHandler   *actionHandler;

} MQTTSettings;

/*-----------------------------------------------------------------------------
 * Lib mqtt exposed functions
 *
 *---------------------------------------------------------------------------*/
void lib_mqtt_RegisterCallbacks(MQTTPublisher *mqttPublisher,
                                MQTTActionHandler *actionHandler);
int lib_mqtt_PublishMessage(const int &topicID, 
							const String message,
							String dest = LIB_MQTT_TOPIC_SERVER_STR);
int lib_mqtt_PublishMessage(const String subTopic,
                            const int &topicID, 
                            const String message,
							String dest = LIB_MQTT_TOPIC_SERVER_STR);
int lib_mqtt_PublishReply(const String subTopic, MQTTMessage mqttMessage);
int lib_mqtt_AppendMQTTAction(const MQTTMessageAction &MessageAction);
int lib_mqtt_GetDoubleMessageArguments(const String Message, 
                                       String &argumentOne, 
                                       String &argumentTwo);

/*---------------------------------------------------------------------------*/
#endif /* LIB_MQTT_H											             */
/*---------------------------------------------------------------------------*/