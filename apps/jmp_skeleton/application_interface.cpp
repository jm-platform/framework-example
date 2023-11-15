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
 * Application interface include files
 *
 *---------------------------------------------------------------------------*/
#include "application_info.h"

/*-----------------------------------------------------------------------------
 * The one and only application module interface variable define
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface *moduleInterfaces[] =
{
	/* Mandatory module interface pointers */
	&mod_debug_ModuleInterface,

	/* Optional module interface pointers */
	&mod_mqtt_client_ModuleInterface,
	&mod_skeleton_ModuleInterface,

	/* Application interface pointers */
	&app_skeleton_ModuleInterface,

	/* The last pointer always a NULL pointer */
	(ModuleInterface*)NULL
};


/*---------------------------------------------------------------------------*/
