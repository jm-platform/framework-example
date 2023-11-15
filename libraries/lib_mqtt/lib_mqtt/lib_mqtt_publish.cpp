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
 * Lib MQTT publish include times
 *
 *---------------------------------------------------------------------------*/
#include "lib_mqtt_publish.h" 

/*-----------------------------------------------------------------------------
 * Lib MQTT publish message functions
 *
 *---------------------------------------------------------------------------*/
int lib_mqtt_publish_PublishDeviceStatus(const String &name, 
										 const String &icon, 
										 const String &value)
{
	int returnCode = RETURN_CODE_OK;

	if ((value == LIB_DEFINE_STATUS_OK_STR) || (value == LIB_DEFINE_STATUS_WARNING_STR) || 
		(value == LIB_DEFINE_STATUS_ERROR_STR))
	{
		String message = "status:" +
						 name + LIB_MQTT_ARGUMENT_SEPERATOR_STR +
						 icon + LIB_MQTT_ARGUMENT_SEPERATOR_STR +
						 value;

		returnCode = lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_VALUE, message);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_publish_PublishDeviceData(const String &name, 
									   const String &icon, 
									   const String &value,
									   const String &notation, 
									   const String &unit)
{
	int returnCode = RETURN_CODE_OK;

	if((unit == LIB_DEFINE_VALUE_UNIT_BOOL_STR) || (unit == LIB_DEFINE_VALUE_UNIT_STRING_STR) ||
	   (unit == LIB_DEFINE_VALUE_UNIT_FLOAT_STR) || (unit == LIB_DEFINE_VALUE_UNIT_INT_STR))
	{
		String message = "data:" +
					 	 name + LIB_MQTT_ARGUMENT_SEPERATOR_STR +
					 	 icon + LIB_MQTT_ARGUMENT_SEPERATOR_STR +
					 	 value + LIB_MQTT_ARGUMENT_SEPERATOR_STR +
						 notation + LIB_MQTT_ARGUMENT_SEPERATOR_STR +
						 unit;

		returnCode = lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_VALUE, message);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_publish_PublishNotification(const String &level, 
										 const String &notification)
{
	String message = level + LIB_MQTT_ARGUMENT_SEPERATOR_STR + notification;
	
	return lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_NOTIFICATION, message);
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_publish_PublishLocation(const float &latitude, 
									 const float &longitude,
									 const float &accuracy)
{
	std::ostringstream messageStream;

	messageStream << latitude << LIB_MQTT_ARGUMENT_SEPERATOR_STR
				  << longitude << LIB_MQTT_ARGUMENT_SEPERATOR_STR
				  << accuracy;
	String message = String(messageStream.str());
	
	return lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_LOCATION, message);
}

/*---------------------------------------------------------------------------*/
int lib_mqtt_publish_PublishLocation(const String &latitude, 
									 const String &longitude,
									 const String &accuracy)
{
	std::ostringstream messageStream;

	messageStream << latitude << LIB_MQTT_ARGUMENT_SEPERATOR_STR
				  << longitude << LIB_MQTT_ARGUMENT_SEPERATOR_STR
				  << accuracy;
	String message = String(messageStream.str());
	
	return lib_mqtt_PublishMessage(LIB_MQTT_TOPIC_RESOURCE_LOCATION, message);
}

/*---------------------------------------------------------------------------*/