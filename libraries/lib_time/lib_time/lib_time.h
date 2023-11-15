/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_TIME_H
#define LIB_TIME_H

/*-----------------------------------------------------------------------------
 * Lib time include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include "../../lib_defines/lib_defines.h"

/*---------------------------------------------------------------------------*/
#include "../../../modules/mod_debug/mod_debug.h"

/*-----------------------------------------------------------------------------
 * Lib time define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_TIME_TIMER_ID_MIN			            SIGRTMIN
#define LIB_TIME_TIMER_ID_MAX			            SIGRTMAX

/*---------------------------------------------------------------------------*/
#define LIB_TIME_TIMER_1HOUR_VALUE			        3600000
#define LIB_TIME_TIMER_1MIN_VALUE			        60000
#define LIB_TIME_TIMER_1HZ_VALUE			        1000
#define LIB_TIME_TIMER_1SEC_VALUE			        LIB_TIME_TIMER_1HZ_VALUE
#define LIB_TIME_TIMER_10HZ_VALUE			        100
#define LIB_TIME_TIMER_100HZ_VALUE			        10
#define LIB_TIME_TIMER_1000HZ_VALUE			        1

/*-----------------------------------------------------------------------------
 * Lib time timer ID
 *
 *---------------------------------------------------------------------------*/
#define LIB_TIME_ID_APP_SKELETON_1HZ				(LIB_TIME_TIMER_ID_MIN + 1)
#define LIB_TIME_ID_MOD_MQTT_CLIENT_1HZ				(LIB_TIME_TIMER_ID_MIN + 2)
#define LIB_TIME_ID_APP_DEVICE_1HZ				    (LIB_TIME_TIMER_ID_MIN + 3)
#define LIB_TIME_ID_APP_TRACKER_1HZ				    (LIB_TIME_TIMER_ID_MIN + 4)
#define LIB_TIME_ID_APP_SERVER_1HZ				    (LIB_TIME_TIMER_ID_MIN + 5)
#define LIB_TIME_ID_MOD_SKELETON_1HZ				(LIB_TIME_TIMER_ID_MIN + 6)
#define LIB_TIME_ID_MOD_IO_1HZ				        (LIB_TIME_TIMER_ID_MIN + 7)
#define LIB_TIME_ID_MOD_MQTT_SERVER_1HZ				(LIB_TIME_TIMER_ID_MIN + 8)
#define LIB_TIME_ID_MOD_MONGODB_1HZ				    (LIB_TIME_TIMER_ID_MIN + 9)
#define LIB_TIME_ID_MOD_GPRS_1HZ				    (LIB_TIME_TIMER_ID_MIN + 10)
#define LIB_TIME_ID_MOD_NOTIFICATIONS_1HZ			(LIB_TIME_TIMER_ID_MIN + 11)
#define LIB_TIME_ID_MOD_GPS_1HZ				        (LIB_TIME_TIMER_ID_MIN + 12)
#define LIB_TIME_ID_MOD_SYNTHESIZER_100HZ		    (LIB_TIME_TIMER_ID_MIN + 13)
#define LIB_TIME_ID_APP_SONIFIER_1HZ		        (LIB_TIME_TIMER_ID_MIN + 14)
#define LIB_TIME_ID_MOD_PUPPET_DEVICE_1HZ           (LIB_TIME_TIMER_ID_MIN + 15)
#define LIB_TIME_ID_APP_PUPPET_1HZ				    (LIB_TIME_TIMER_ID_MIN + 17)
#define LIB_TIME_ID_APP_SENSOR_HUB_1HZ			    (LIB_TIME_TIMER_ID_MIN + 17)
#define LIB_TIME_ID_APP_JMSCRIPT_PARSER_1HZ		    (LIB_TIME_TIMER_ID_MIN + 19)
#define LIB_TIME_ID_APP_TERMINAL_1HZ		    	(LIB_TIME_TIMER_ID_MIN + 20)
#define LIB_TIME_ID_APP_MOCK_1HZ		    		(LIB_TIME_TIMER_ID_MIN + 21)
#define LIB_TIME_ID_MOD_DEBUG_1HZ		    		(LIB_TIME_TIMER_ID_MIN + 22)

/*-----------------------------------------------------------------------------
 * Lib time exposed functions
 *
 *---------------------------------------------------------------------------*/
String lib_time_GetDateString();
String lib_time_GetDateTimeString();
String lib_time_GetDateTimeStringISO();

/*---------------------------------------------------------------------------*/
bool lib_time_SetupAndStartTimer(int sig, uint16_t milliSecs, void *handlerFunction, timer_t *timerId);
void lib_time_StopTimer(timer_t timerId);

/*---------------------------------------------------------------------------*/
#endif /* LIB_TIME_H											             */
/*---------------------------------------------------------------------------*/