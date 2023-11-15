/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_GPS_IMP_H
#define MOD_GPS_IMP_H

/*-----------------------------------------------------------------------------
 * Gps include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Gps define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPS_ARGUMENT_LATITUDE                   2
#define MOD_GPS_ARGUMENT_LONGITUDE                  4
#define MOD_GPS_ARGUMENT_SATTALLITES                7
#define MOD_GPS_ARGUMENT_ACCURACY                   8

/*-----------------------------------------------------------------------------
 * Gps ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPS_SETTING_SERIAL_PORT_STR_LENGTH_MIN              1
#define MOD_GPS_SETTING_SERIAL_PORT_STR_LENGTH_MAX              255

/*---------------------------------------------------------------------------*/
#define MOD_GPS_SETTING_SERIAL_BAUDRATE_MIN                     0
#define MOD_GPS_SETTING_SERIAL_BAUDRATE_MAX                     4000000

/*---------------------------------------------------------------------------*/
#define MOD_GPS_SETTING_LOCATION_UPDATE_INTERVAL_MIN            -1
#define MOD_GPS_SETTING_LOCATION_UPDATE_INTERVAL_MAX            3600

/*---------------------------------------------------------------------------*/
#define MOD_GPS_LATITUDE_DEGREE_MIN                             0.0
#define MOD_GPS_LATITUDE_DEGREE_MAX                             90.0

/*---------------------------------------------------------------------------*/
#define MOD_GPS_LONGITUDE_DEGREE_MIN                            0.0
#define MOD_GPS_LONGITUDE_DEGREE_MAX                            180.0

/*---------------------------------------------------------------------------*/
#define MOD_GPS_MINUTES_MIN                                     0.0
#define MOD_GPS_MINUTES_MAX                                     61.0

/*---------------------------------------------------------------------------*/
#define MOD_GPS_SATTALLITES_MIN                                 0
#define MOD_GPS_SATTALLITES_MAX                                 100

/*---------------------------------------------------------------------------*/
#define MOD_GPS_ACCURACY_MIN                                    0.0

/*-----------------------------------------------------------------------------
 * Gps initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPS_INITIAL_SERIAL_PORT_STR                         "/dev/ttyUSB0"
#define MOD_GPS_INITIAL_SERIAL_BAUDRATE                         9600
#define MOD_GPS_INITIAL_LOCATION_UPDATE_INTERVAL                3 // Seconds

/*-----------------------------------------------------------------------------
 * Gps settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPS_SETTING_SERIAL_PORT_STR   			            "gps-serial-port"
#define MOD_GPS_SETTING_SERIAL_BAUDRATE_STR   	                "gps-serial-baudrate"
#define MOD_GPS_SETTING_LOCATION_UPDATE_INTERVAL_STR            "gps-location-update-interval"

/*-----------------------------------------------------------------------------
 * Gps type definitions
 *
 *---------------------------------------------------------------------------*/
typedef std::list <String> ReplyList;

/*---------------------------------------------------------------------------*/
typedef struct GpsLocation
{
    double  latitude;
    double  longitude;
    double  accuracy;
    int     sattallites;

} GpsLocation;

/*---------------------------------------------------------------------------*/
typedef void LocationCallback(GpsLocation&);

/*-----------------------------------------------------------------------------
 * Gps settings
 *
 *---------------------------------------------------------------------------*/
typedef struct GpsSettings
{
    String  serialPort;
    int     serialBaudrate;

    int     locationUpdateInterval;

} GpsSettings;

/*-----------------------------------------------------------------------------
 * Gps workers
 *
 *---------------------------------------------------------------------------*/
typedef struct GpsWorkers
{
    timer_t             timer1HzId;
    bool                timer1HzTrigger;
    int                 locationUpdateTimer;

    int                 serialConnection;

    ReplyList           replyList;
    bool                getReply;

    GpsLocation         gpsLocation;
    LocationCallback    *locationCallback;

} GpsWorkers;

/*-----------------------------------------------------------------------------
 * Gps exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_gps_PreInitialisation();
int  mod_gps_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_gps_PostInitialisation();
int  mod_gps_DestroyData();
int  mod_gps_MainloopFunction();
int  mod_gps_TestFunction();

/*---------------------------------------------------------------------------*/
void mod_gps_RegisterLocationCallback(LocationCallback *locationCallback);

/*---------------------------------------------------------------------------*/
#endif /* MOD_GPS_IMP_H											             */
/*---------------------------------------------------------------------------*/
