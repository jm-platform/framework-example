/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MONGODB_DEFINES_H
#define MOD_MONGODB_DEFINES_H

/*-----------------------------------------------------------------------------
 * mod MongoDB defines include files
 *---------------------------------------------------------------------------*/
#include "../../../libraries/gcc.h"
#include "../../../libraries/libs.h"

/*---------------------------------------------------------------------------*/
#include <mongoc/mongoc.h>
#include <json.h>

/*---------------------------------------------------------------------------*/
#include "../../mod_debug/mod_debug.h"

/*-----------------------------------------------------------------------------
 * mod MongoDB defines define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_MONGODB_DATABASE_NAME_DEVICES                   	"JM-Platform"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_COLLECTION_NAME_DEVICES                 	"Devices"
#define MOD_MONGODB_COLLECTION_NAME_DEVICE_DATA             	"DeviceData"
#define MOD_MONGODB_COLLECTION_NAME_ACTIONS            			"DeviceActions"
#define MOD_MONGODB_COLLECTION_NAME_VALUE            			"DeviceValues"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_DEVICE_ID                        "deviceId"
#define MOD_MONGODB_VALUE_NAME_DEVICE_GROUP                     "deviceGroup"
#define MOD_MONGODB_VALUE_NAME_DEVICE_NAME                      "deviceName"
#define MOD_MONGODB_VALUE_NAME_DEVICE_ONLINE                    "online"
#define MOD_MONGODB_VALUE_NAME_DEVICE_LAST_MODIFIED             "lastModified"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_TYPE             	"type"
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_NAME             	"name"
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_DATE             	"date"
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_ICON             	"icon"
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_VALUE             	"value"
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_UNIT             	"unit"
#define MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_NOITATION          	"notation"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_DIGITAL_INPUT        "digitalInput"
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LATITUDE             "latitude"
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LONGITUDE            "longitude"
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_ACCURACY             "locationAccuracy"
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LOCATION_MODIFIED    "locationLastModified"
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_NOTIFICATIONS        "notifications"
#define MOD_MONGODB_VALUE_NAME_DEVICE_DATA_EMAILS               "emailNotifications"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_NOTIFICATIONS_DATE               "time"
#define MOD_MONGODB_VALUE_NAME_NOTIFICATIONS_LEVEL              "level"
#define MOD_MONGODB_VALUE_NAME_NOTIFICATIONS_MESSAGE            "message"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_DEVICE_ACTION_ALARM              "alarm"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_SUB_DEVICES                      "subDevices"
#define MOD_MONGODB_VALUE_NAME_SENSORS                          "sensors"
#define MOD_MONGODB_VALUE_NAME_STATUS                           "status"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_LATITUDE                         "latitude"
#define MOD_MONGODB_VALUE_NAME_LONGITUDE                        "longitude"
#define MOD_MONGODB_VALUE_NAME_HEARTBEAT                        "heartbeat"
#define MOD_MONGODB_VALUE_NAME_BODY_TEMPERATURE                 "bodyTemperature"
#define MOD_MONGODB_VALUE_NAME_BLOOD_SATURATION                 "bloodSaturation"
#define MOD_MONGODB_VALUE_NAME_AMBIENT_NOISE                    "ambientNoise"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_VALUE_NAME_OVERAL_STATUS                    "overalStatus"
#define MOD_MONGODB_VALUE_NAME_SUPPORT                          "support"
#define MOD_MONGODB_VALUE_NAME_PROXIMITY_RANGE                  "proximityRange"
#define MOD_MONGODB_VALUE_NAME_CONDITION                        "physicalCondition"
#define MOD_MONGODB_VALUE_NAME_IN_COMBAT                        "inCombat"
#define MOD_MONGODB_VALUE_NAME_EMERGENCY                        "emergency"

/*---------------------------------------------------------------------------*/
#define MOD_MONGODB_NOTIFICATION_EMAIL_ADRESSES_MAX             255

/*-----------------------------------------------------------------------------
 * mod MongoDB defines type definitions
 *
 *---------------------------------------------------------------------------*/
typedef struct DBDevice 
{
    String deviceId;
    String deviceGroup;
    String deviceName;

    bool   online;

    String lastModified;

} DBDevice;

/*---------------------------------------------------------------------------*/
typedef struct DBDeviceData
{
    bool    digitalInput;
    String  notificationEmails[MOD_MONGODB_NOTIFICATION_EMAIL_ADRESSES_MAX];
    int     emailsAmount;

} DBDeviceData;

/*---------------------------------------------------------------------------*/
typedef struct DBDeviceActions
{
    String   alarm;

} DBDeviceActions;

/*---------------------------------------------------------------------------*/
typedef struct DBConnection
{
    mongoc_client_t     *mongoClient;
    mongoc_uri_t        *mongoURI;
    mongoc_database_t   *database;
	mongoc_collection_t *collection;

} DBConnection;

/*-----------------------------------------------------------------------------
 * MongoDB settings
 *
 *---------------------------------------------------------------------------*/
typedef struct MongoDBSettings
{
    String mongoDBUri;

} MongoDBSettings;

/*-----------------------------------------------------------------------------
 * MongoDB workers
 *
 *---------------------------------------------------------------------------*/
typedef struct MongoDBWorkers
{
    timer_t timer1HzId;
    bool    timer1HzTrigger;

    DBConnection dbConnection;

} MongoDBWorkers;
                                             
/*---------------------------------------------------------------------------*/
#endif /* MOD_MONGODB_DEFINES_H											     */
/*---------------------------------------------------------------------------*/