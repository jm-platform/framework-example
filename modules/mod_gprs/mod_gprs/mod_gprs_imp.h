/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_GPRS_IMP_H
#define MOD_GPRS_IMP_H

/*-----------------------------------------------------------------------------
 * GPRS include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*---------------------------------------------------------------------------*/
#include "mod_gprs_message.h" 

/*-----------------------------------------------------------------------------
 * GPRS define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPRS_COMMAND_REQUEST_STATUS_STR                     "AT\r\n"
#define MOD_GPRS_COMMAND_REQUEST_DISABLE_ECHO_STR               "ATE0&W\r\n"
#define MOD_GPRS_COMMAND_REQUEST_ENABLE_ECHO_STR                "ATE1\r\n"
#define MOD_GPRS_COMMAND_REQUEST_LOCATION_STR                   "AT+CLBS=1,1\r\n"
#define MOD_GPRS_COMMAND_REQUEST_IP_STR                         "AT+SAPBR=2,1\r\n"

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_COMMAND_SET_FUNCTIONALITY_STR                  "AT+CFUN=1\r\n"
#define MOD_GPRS_COMMAND_SET_READY_STR                          "AT+CPIN?\r\n"
#define MOD_GPRS_COMMAND_SET_CONNECTION_TYPE_STR                "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n"
#define MOD_GPRS_COMMAND_SET_APN_STR                            "AT+SAPBR=3,1,\"APN\",\"portalmmm.nl\"\r\n"
#define MOD_GPRS_COMMAND_SET_GPRS_MODE_ON_STR                   "AT+SAPBR=1,1\r\n"
#define MOD_GPRS_COMMAND_SET_GPRS_MODE_STR                      "AT+CIICR\r\n"

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_COMMAND_STATUS_OK_STR                          "OK"
#define MOD_GPRS_COMMAND_STATUS_ERROR_STR                       "ERROR"
#define MOD_GPRS_COMMAND_LOCATION_STR                           "+CLBS:"
#define MOD_GPRS_COMMAND_IP_STR                                 "+SAPBR:"

/*-----------------------------------------------------------------------------
 * GPRS ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPRS_SETTING_SERIAL_PORT_STR_LENGTH_MIN             1
#define MOD_GPRS_SETTING_SERIAL_PORT_STR_LENGTH_MAX             255

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_SETTING_SERIAL_BAUDRATE_MIN                    0
#define MOD_GPRS_SETTING_SERIAL_BAUDRATE_MAX                    4000000

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_SETTING_LOCATION_UPDATE_INTERVAL_MIN           1
#define MOD_GPRS_SETTING_LOCATION_UPDATE_INTERVAL_MAX           3600

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_SETTING_STARTUP_DELAY_MIN                      0
#define MOD_GPRS_SETTING_STARTUP_DELAY_MAX                      60

/*-----------------------------------------------------------------------------
 * GPRS initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_INITIAL_SERIAL_PORT_STR                             "/dev/ttyS0"
#define MOD_INITIAL_SERIAL_BAUDRATE                             9600
#define MOD_INITIAL_LOCATION_UPDATE_INTERVAL                    10 // Seconds
#define MOD_INITIAL_STARTUP_DELAY                               5 // Seconds

/*-----------------------------------------------------------------------------
 * GPRS settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPRS_SETTING_SERIAL_PORT_STR   			            "gprs-serial-port"
#define MOD_GPRS_SETTING_SERIAL_BAUDRATE_STR   	                "gprs-serial-baudrate"
#define MOD_GPRS_SETTING_LOCATION_UPDATE_INTERVAL_STR           "gprs-location-update-interval"
#define MOD_GPRS_SETTING_STARTUP_DELAY_STR                      "gprs-startup-delay"

/*-----------------------------------------------------------------------------
 * GPRS type definitions settings
 *
 *---------------------------------------------------------------------------*/
typedef std::list <String> ReplyList;

/*---------------------------------------------------------------------------*/
typedef struct LocationData
{
    String  latitude;
    String  longitude;
    String  accuracy;

} LocationData;

/*-----------------------------------------------------------------------------
 * GPRS settings
 *
 *---------------------------------------------------------------------------*/
typedef struct GPRSSettings
{
    String  serialPort;
    int     serialBaudrate;

    int     locationUpdateInterval;
    int     startupDelay;

} GPRSSettings;

/*-----------------------------------------------------------------------------
 * GPRS workers
 *
 *---------------------------------------------------------------------------*/
typedef struct GPRSWorkers
{
    timer_t             timer1HzId;
    bool                timer1HzTrigger;

    int                 serialConnection;
    int                 serialReadDelay;

    int                 locationUpdateTimer;
    int                 replyTimer;

    ReplyList           replyList;
    bool                getReply;

    bool                startUp;
    int                 startUpTimer;

    LocationData        locationData;

} GPRSWorkers;

/*-----------------------------------------------------------------------------
 * GPRS exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_gprs_PreInitialisation();
int  mod_gprs_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_gprs_PostInitialisation();
int  mod_gprs_DestroyData();
int  mod_gprs_MainloopFunction();
int  mod_gprs_TestFunction();

/*---------------------------------------------------------------------------*/
#endif /* MOD_GPRS_IMP_H											         */
/*---------------------------------------------------------------------------*/
