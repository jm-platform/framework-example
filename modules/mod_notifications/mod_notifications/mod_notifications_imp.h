/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_NOTIFICATIONS_IMP_H
#define MOD_NOTIFICATIONS_IMP_H

/*-----------------------------------------------------------------------------
 * Notifications include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Notifications define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_NOTIFICATIONS_LEVEL_NOTICE                              "notice"
#define MOD_NOTIFICATIONS_LEVEL_ALERT                               "alert"

/*---------------------------------------------------------------------------*/
#define MOD_NOTIFICATIONS_MESSAGE_ONLINE                            "Device online"
#define MOD_NOTIFICATIONS_MESSAGE_OFFLINE                           "Device offline"
#define MOD_NOTIFICATIONS_MESSAGE_ALARM                             "Alarm is "

/*-----------------------------------------------------------------------------
 * Notifications ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_NOTIFICATIONS_SETTING_MAIL_FOLDER_STR_LENGTH_MIN        1
#define MOD_NOTIFICATIONS_SETTING_MAIL_FOLDER_STR_LENGTH_MAX        255

/*-----------------------------------------------------------------------------
 * Notifications initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_NOTIFICATIONS_INITIAL_MAIL_FOLDER_STR   			    "/home/jmserver/jm-platform-v2.0/mail/alarm-mail.txt"

/*-----------------------------------------------------------------------------
 * Notifications settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_NOTIFICATIONS_SETTING_MAIL_FOLDER_STR   			    "alarm-mail-path"

/*-----------------------------------------------------------------------------
 * Notifications settings
 *
 *---------------------------------------------------------------------------*/
typedef struct NotificationsSettings
{
    String mailFolderPath;

} NotificationsSettings;

/*-----------------------------------------------------------------------------
 * Notifications workers
 *
 *---------------------------------------------------------------------------*/
typedef struct NotificationsWorkers
{
    timer_t timer1HzId;
    bool    timer1HzTrigger;

    String  notificationEmails[255];

} NotificationsWorkers;

/*-----------------------------------------------------------------------------
 * Notifications exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_notifications_PreInitialisation();
int  mod_notifications_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_notifications_PostInitialisation();
int  mod_notifications_DestroyData();
int  mod_notifications_MainloopFunction();
int  mod_notifications_TestFunction();

/*---------------------------------------------------------------------------*/
int mod_notifications_NotifyOnline(const String &deviceId);
int mod_notifications_NotifyOffline(const String &deviceId);
int mod_notifications_NotifyAlarmState(const String &deviceId, const String &alarm);
int mod_notifications_SendAlarmEmails(const String &deviceId);
int mod_notifications_NotifyGeneral(const String &deviceId, 
									const String &level,
									const String &message);
/*---------------------------------------------------------------------------*/
#endif /* MOD_NOTIFICATIONS_IMP_H											 */
/*---------------------------------------------------------------------------*/
