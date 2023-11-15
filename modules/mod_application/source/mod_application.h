/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_APPLICATION_H
#define MOD_APPLICATION_H

/*-----------------------------------------------------------------------------
 * Application module include files
 *
 *---------------------------------------------------------------------------*/
#include "../../../libraries/gcc.h"
#include "../../../libraries/libs.h"

/*---------------------------------------------------------------------------*/
#include "../../mod_debug/mod_debug.h"

/*-----------------------------------------------------------------------------
 * Application module defines
 *
 *---------------------------------------------------------------------------*/
#define MOD_APPLICATION_ARGUMENT_INIFILE_PATH               "inifile"
#define MOD_APPLICATION_ARGUMENT_DEBUG_MODE                 "debug"

/*---------------------------------------------------------------------------*/
#define APPLICATION_PROCESS_SLEEP_INTERVAL				    5000000	// nano seconds = 5ms

/*-----------------------------------------------------------------------------
 * Application module settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_APPLICATION_INITIAL_INIFILE_PATH                "../ini/jmp-settings.ini"

/*-----------------------------------------------------------------------------
 * Application module settings
 *
 *---------------------------------------------------------------------------*/
typedef struct ApplicationSettings
{
    String    inifilePath;

} ApplicationSettings;

typedef struct ApplicationWorkers
{
    struct timespec sleepTimeRequest;
	struct timespec sleepTimeRemain;

    bool      applicationContinue;

} ApplicationWorkers;

/*---------------------------------------------------------------------------*/
#endif /* MOD_APPLICATION_H											         */
/*---------------------------------------------------------------------------*/
