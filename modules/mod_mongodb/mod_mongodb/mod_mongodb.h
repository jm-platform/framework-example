/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_MONGODB_H
#define MOD_MONGODB_H

/*-----------------------------------------------------------------------------
 * MongoDB include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * MongoDB exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_mongodb_SetMongoDBUri(SettingsObjectData &SettingsObjectData);

/*-----------------------------------------------------------------------------
 * MongoDB module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_mongodb_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_MONGODB_H											             */
/*---------------------------------------------------------------------------*/
