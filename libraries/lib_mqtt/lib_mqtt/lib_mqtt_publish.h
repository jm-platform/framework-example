/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_MQTT_PUBLISH_H
#define LIB_MQTT_PUBLISH_H

/*-----------------------------------------------------------------------------
 * Lib MQTT publish include files
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Lib MQTT publish define values
 *
 *---------------------------------------------------------------------------*/
#include "lib_mqtt.h" 

/*-----------------------------------------------------------------------------
 * Lib MQTT publish type definitions
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Lib MQTT publish exposed functions
 *
 *---------------------------------------------------------------------------*/
int lib_mqtt_publish_PublishDeviceStatus(const String &name, 
										 const String &icon, 
										 const String &value);
int lib_mqtt_publish_PublishDeviceData(const String &name, 
									   const String &icon, 
									   const String &value,
									   const String &notation, 
									   const String &unit);
int lib_mqtt_publish_PublishNotification(const String &level, 
										 const String &notification);

/*---------------------------------------------------------------------------*/											
int lib_mqtt_publish_PublishLocation(const float &latitude, 
									 const float &longitude,
									 const float &accuracy);
int lib_mqtt_publish_PublishLocation(const String &latitude, 
									 const String &longitude,
									 const String &accuracy);
									 
/*---------------------------------------------------------------------------*/
#endif /* LIB_MQTT_PUBLISH_H											 	 */
/*---------------------------------------------------------------------------*/
