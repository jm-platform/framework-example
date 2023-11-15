/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MONGODB_IMP_H
#define MOD_MONGODB_IMP_H

/*-----------------------------------------------------------------------------
 * MongoDB include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*---------------------------------------------------------------------------*/
#include "mod_db_connection.h" 
#include "mod_db_device.h" 
#include "mod_mongodb_defines.h" 

/*-----------------------------------------------------------------------------
 * MongoDB define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_MONGODB_SYSTEM_NAME                             "jmp-mqtt-server"

/*-----------------------------------------------------------------------------
 * MongoDB database define values
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * MongoDB ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_MONGODB_SETTING_URI_STR_LENGTH_MIN              1
#define MOD_MONGODB_SETTING_URI_STR_LENGTH_MAX              255

/*-----------------------------------------------------------------------------
 * MongoDB initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_MONGODB_INITIAL_URI_STR                         "mongodb://localhost:27017"

/*-----------------------------------------------------------------------------
 * MongoDB settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_MONGODB_SETTING_URI_STR   			            "mongodb-uri"

/*-----------------------------------------------------------------------------
 * MongoDB exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_PreInitialisation();
int  mod_mongodb_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_mongodb_PostInitialisation();
int  mod_mongodb_DestroyData();
int  mod_mongodb_MainloopFunction();
int  mod_mongodb_TestFunction();

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
#endif /* MOD_MONGODB_IMP_H											         */
/*---------------------------------------------------------------------------*/
