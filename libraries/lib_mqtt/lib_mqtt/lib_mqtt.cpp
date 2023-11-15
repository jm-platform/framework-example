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
 * Lib mqtt include files
 *
 *---------------------------------------------------------------------------*/
#include "lib_mqtt.h" 

/*-----------------------------------------------------------------------------
 * MQTT settings
 *
 *---------------------------------------------------------------------------*/
MQTTSettings mqttSettings;

/*-----------------------------------------------------------------------------
 * Lib mqtt functions
 *
 *---------------------------------------------------------------------------*/
void lib_mqtt_InitMQTTMessage(MQTTMessage &mqttMessage)
{
    mqttMessage.messageSource.clear();
    mqttMessage.messageDest.clear();
    mqttMessage.messageTopic.clear();
    mqttMessage.messageSubTopic.clear();
    mqttMessage.messageData.clear();

    mqttMessage.messageTopicID = 0;
}

/*---------------------------------------------------------------------------*/
void lib_mqtt_RegisterCallbacks(MQTTPublisher *mqttPublisher,
                                MQTTActionHandler *actionHandler)
{
    mqttSettings.mqttPublisher = mqttPublisher;
    mqttSettings.actionHandler = actionHandler;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_PublishMessage(const int &topicID, 
							const String message,
							String dest)
{
    int returnCode = RETURN_CODE_FAIL;

    if (mqttSettings.mqttPublisher != NULL) 
    {
        MQTTMessage mqttMessage;
        lib_mqtt_InitMQTTMessage(mqttMessage);

        mqttMessage.messageTopicID  = topicID;
        mqttMessage.messageDest     = dest;
        mqttMessage.messageData     = message;

        returnCode = (mqttSettings.mqttPublisher)(mqttMessage);
    }
    
    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_PublishMessage(const String subTopic,
                            const int &topicID, 
                            const String message,
							String dest)
{
    int returnCode = RETURN_CODE_FAIL;

    if (mqttSettings.mqttPublisher != NULL) 
    {
        MQTTMessage mqttMessage;
        lib_mqtt_InitMQTTMessage(mqttMessage);

        mqttMessage.messageTopicID      = topicID;
        mqttMessage.messageSubTopic     = subTopic;
        mqttMessage.messageDest         = dest;
        mqttMessage.messageData         = message;

        returnCode = (mqttSettings.mqttPublisher)(mqttMessage);
    }
    
    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_PublishReply(const String subTopic, MQTTMessage mqttMessage)
{
    int returnCode = RETURN_CODE_FAIL;

    if (mqttSettings.mqttPublisher != NULL) 
    {
        mqttMessage.messageSubTopic = subTopic;

        String source   = mqttMessage.messageSource;
        String dest     = mqttMessage.messageDest;

        mqttMessage.messageSource   = dest;
        mqttMessage.messageDest     = source;

        returnCode = (mqttSettings.mqttPublisher)(mqttMessage);
    }
    
    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_AppendMQTTAction(const MQTTMessageAction &MessageAction)
{
    int returnCode = RETURN_CODE_FAIL;

    if (mqttSettings.actionHandler != NULL)
    {
        (mqttSettings.actionHandler)(MessageAction);

        returnCode = RETURN_CODE_OK;
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_GetDoubleMessageArguments(const String Message, 
                                       String &argumentOne, 
                                       String &argumentTwo)
{
    int returnCode = RETURN_CODE_FAIL;

    String argValue;

    std::istringstream arg(Message);
    std::getline(arg, argValue, LIB_MQTT_ARGUMENT_SEPERATOR_STR);

    argumentOne = argValue;

    std::getline(arg, argValue);

    if (argumentOne != argValue)
    {
        argumentTwo = argValue;

        returnCode = RETURN_CODE_OK;
    }

    return returnCode;
}

/*-----------------------------------------------------------------------------
 * Lib mqtt MQTTClient class functions
 *
 *---------------------------------------------------------------------------*/
void MQTTClient::registerCallbackFunctions(MQTTConnector *connector,
                                           MQTTSubscriber *subscriber,
                                           MQTTDataHandler *dataHandler,
                                           MQTTConnector *disconnector)
{
    MQTTClient::connector = connector;
    MQTTClient::subscriber = subscriber;
    MQTTClient::dataHandler = dataHandler;
    MQTTClient::disconnector = disconnector;
}

/*---------------------------------------------------------------------------*/
void MQTTClient::on_connect(int returnCode __attribute__((unused)))
{
    MQTTClient::connected = true;

    MQTTClient::connector();
    MQTTClient::subscriber();
}

/*---------------------------------------------------------------------------*/
void MQTTClient::on_disconnect(int returnCode __attribute__((unused)))
{
    MQTTClient::connected = false;

    MQTTClient::disconnector();
}


/*---------------------------------------------------------------------------*/
void MQTTClient::on_message(const struct mosquitto_message *message)
{
    MQTTMessage mqttMessage;

    int returnCode = lib_mqtt_tokenizeTopic(message->topic,
                                            mqttMessage.messageDest, 
                                            mqttMessage.messageSource, 
                                            mqttMessage.messageTopicID,
                                            mqttMessage.messageSubTopic);

    if (returnCode == RETURN_CODE_OK)
    {
        mqttMessage.messageTopic = lib_mqtt_ConvertTopicId(mqttMessage.messageTopicID);

        if (mqttMessage.messageTopic == LIB_MQTT_TOPIC_UNKNOWN_STR)
        {
            returnCode = RETURN_CODE_FAIL;
        }
    }

    if (returnCode == RETURN_CODE_OK)
    {
        if (message->payloadlen > 0)
        {
            mqttMessage.messageData = (char*)message->payload;
        }

        else
        {
            returnCode = RETURN_CODE_FAIL;
        }
    }

    if (returnCode == RETURN_CODE_OK)
    {
        MQTTClient::dataHandler(mqttMessage);
    }

    else
    {
        char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "invalid MQTT message: \"%s\"", message->topic);
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, buffer);
    }
}

/*---------------------------------------------------------------------------*/
void MQTTClient::on_log(int level, const char* logMessage)
{
    if (MQTTClient::logMode == LIB_MQTT_LOG_MODE_ALL_STR)
    {
        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, logMessage);
    }

    else if ((MQTTClient::logMode == LIB_MQTT_LOG_MODE_GENERAL_STR) &&
            (level != MOSQ_LOG_DEBUG))
    {
        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, logMessage);
    }

    else if ((MQTTClient::logMode == LIB_MQTT_LOG_MODE_ERROR_STR) &&
            ((level == MOSQ_LOG_WARNING) || (level == MOSQ_LOG_ERR)))
    {
        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, logMessage);
    }

    else if ((MQTTClient::logMode == LIB_MQTT_LOG_MODE_DEBUG_STR) &&
            (level == MOSQ_LOG_DEBUG))
    {
        debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_MESSAGING, logMessage);
    }
}

/*---------------------------------------------------------------------------*/
