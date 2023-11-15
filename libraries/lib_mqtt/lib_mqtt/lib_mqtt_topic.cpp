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
 * Lib mqtt topic include files
 *
 *---------------------------------------------------------------------------*/
#include "lib_mqtt_topic.h" 
#include "../../lib_file/lib_file.h"

/*-----------------------------------------------------------------------------
 * Lib mqtt topic functions
 *
 *---------------------------------------------------------------------------*/
int lib_mqtt_tokenizeTopic(const char* topic, 
                           String &dest, 
                           String &source, 
                           int &topicID, 
                           String& subTopic)
{
    int returnCode = RETURN_CODE_FAIL;

    String buffer;

    std::istringstream arg(topic);
    std::getline(arg, buffer, LIB_FILE_CHARACTER_DASH);
    dest = buffer;
    std::getline(arg, buffer, LIB_FILE_CHARACTER_DASH);
    source = buffer;

    std::getline(arg, buffer, LIB_FILE_CHARACTER_DASH);
    if (source != buffer)
    {
        returnCode = RETURN_CODE_OK;

        String topicIDString = buffer;
        topicID = stoi(topicIDString);

        std::getline(arg, buffer, LIB_FILE_CHARACTER_DASH);
        if (topicIDString != buffer)
        {
            subTopic = buffer;
        }
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
String lib_mqtt_ConvertTopicId(const int topicID)
{
    String subTopic;

    switch (topicID)
    {
        /* connection topics */
        case LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_REGISTER:
            subTopic = LIB_MQTT_TOPIC_CONNECTIONS_REGISTER_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_CONNECT:
            subTopic = LIB_MQTT_TOPIC_CONNECTIONS_CONNECT_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCONNECT:
            subTopic = LIB_MQTT_TOPIC_CONNECTIONS_DISCONNECT_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_NOTIFICATION:
            subTopic = LIB_MQTT_TOPIC_NOTIFICATION_STR;
            break;
         case LIB_MQTT_TOPIC_RESOURCE_DEBUGGING:
            subTopic = LIB_MQTT_TOPIC_DEBUGGING_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_LOG:
            subTopic = LIB_MQTT_TOPIC_LOG_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_DATA:
            subTopic = LIB_MQTT_TOPIC_DATA_STR;
            break;

        /* general topics */
        case LIB_MQTT_TOPIC_RESOURCE_GENERAL:
            subTopic = LIB_MQTT_TOPIC_GENERAL_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_DEVICE_INFO:
            subTopic = LIB_MQTT_TOPIC_DEVICE_INFO_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_JMP_VERSION:
            subTopic = LIB_MQTT_TOPIC_JMP_VERSION_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_ALARM:
            subTopic = LIB_MQTT_TOPIC_ALARM_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_CONNECTIONS_DISCOVER:
            subTopic = LIB_MQTT_TOPIC_CONNECTIONS_DISCOVER_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_VALUE:
            subTopic = LIB_MQTT_TOPIC_VALUE_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_SET_DATA:
            subTopic = LIB_MQTT_TOPIC_SET_DATA_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_TOGGLE_DATA:
            subTopic = LIB_MQTT_TOPIC_TOGGLE_DATA_STR;
            break;
        
        /* skeleton application */
        case LIB_MQTT_TOPIC_RESOURCE_SKELETON_TEST:
            subTopic = LIB_MQTT_TOPIC_SKELETON_TEST_STR;
            break;

        /* mqtt module */
        case LIB_MQTT_TOPIC_RESOURCE_MQTT_VERSION:
            subTopic = LIB_MQTT_TOPIC_MQTT_VERSION_STR;
            break;

        /* io module */
        case LIB_MQTT_TOPIC_RESOURCE_TOGGLE_OUTPUT:
            subTopic = LIB_MQTT_TOPIC_TOGGLE_OUTPUT_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_SET_OUTPUT:
            subTopic = LIB_MQTT_TOPIC_SET_OUTPUT_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_DIGITAL_INPUT:
            subTopic = LIB_MQTT_TOPIC_DIGITAL_INPUT_STR;
            break;

        /* gprs module */
        case LIB_MQTT_TOPIC_RESOURCE_LOCATION:
            subTopic = LIB_MQTT_TOPIC_LOCATION_STR;
            break;

         /* synthesizer module */
        case LIB_MQTT_TOPIC_RESOURCE_AUDIO_SETTINGS:
            subTopic = LIB_MQTT_TOPIC_AUDIO_SETTINGS_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_PLAY_PAUSE:
            subTopic = LIB_MQTT_TOPIC_PLAY_PAUSE_STR;
            break;
            

        /* puppet device module */
        case LIB_MQTT_TOPIC_RESOURCE_PUPPET_DEVICES:
            subTopic = LIB_MQTT_TOPIC_PUPPET_DEVICES_STR;
            break;

         /* sonifier application */
        case LIB_MQTT_TOPIC_RESOURCE_SENSOR_SETTINGS:
            subTopic = LIB_MQTT_TOPIC_SENSOR_SETTINGS_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_ACCEL_X:
            subTopic = LIB_MQTT_TOPIC_ACCEL_X_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_ACCEL_Y:
            subTopic = LIB_MQTT_TOPIC_ACCEL_Y_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_ACCEL_Z:
            subTopic = LIB_MQTT_TOPIC_ACCEL_Z_STR;
            break;

        case LIB_MQTT_TOPIC_RESOURCE_GYRO_X:
            subTopic = LIB_MQTT_TOPIC_GYRO_X_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_GYRO_Y:
            subTopic = LIB_MQTT_TOPIC_GYRO_Y_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_GYRO_Z:
            subTopic = LIB_MQTT_TOPIC_GYRO_Z_STR;
            break;
        
        /* sensor hub application */
        case LIB_MQTT_TOPIC_RESOURCE_SUB_DEVICE_ONLINE:
            subTopic = LIB_MQTT_TOPIC_SUB_DEVICE_ONLINE_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_SUB_DEVICE_SENSORS:
            subTopic = LIB_MQTT_TOPIC_SUB_DEVICE_SENSORS_STR;
            break;
        case LIB_MQTT_TOPIC_RESOURCE_SUB_DEVICE_STATUS:
            subTopic = LIB_MQTT_TOPIC_SUB_DEVICE_STATUS_STR;
            break;


        /* unknown */
        default:
            subTopic = LIB_MQTT_TOPIC_UNKNOWN_STR;
            break;
    }

    return subTopic;
}

/*---------------------------------------------------------------------------*/
String lib_mqtt_CreateTopic(const String source, const String dest, const int topicID)
{
    String topic;

    String topicName = lib_mqtt_ConvertTopicId(topicID);

    if (topicName != LIB_MQTT_TOPIC_UNKNOWN_STR)
    {
        topic.append(dest);
        topic.append(LIB_MQTT_TOPIC_SEPERATOR_STR);
        topic.append(source);
        topic.append(LIB_MQTT_TOPIC_SEPERATOR_STR);
        topic.append(std::to_string(topicID));
    }

    else
    {
        topic = LIB_MQTT_TOPIC_INVALID_STR;
    }

    return topic;
}

/*---------------------------------------------------------------------------*/
String lib_mqtt_CreateTopic(const String source, 
                            const String dest, 
                            const int topicID, 
                            const String subTopic)
{
    String topic;

    String topicName = lib_mqtt_ConvertTopicId(topicID);

    if (topicName != LIB_MQTT_TOPIC_UNKNOWN_STR)
    {
        topic.append(dest);
        topic.append(LIB_MQTT_TOPIC_SEPERATOR_STR);
        topic.append(source);
        topic.append(LIB_MQTT_TOPIC_SEPERATOR_STR);
        topic.append(std::to_string(topicID));
        topic.append(LIB_MQTT_TOPIC_SEPERATOR_STR);
        topic.append(subTopic);
    }

    else
    {
        topic = LIB_MQTT_TOPIC_INVALID_STR;
    }

    return topic;
}

/*---------------------------------------------------------------------------*/