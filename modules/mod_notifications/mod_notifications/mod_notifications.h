/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_NOTIFICATIONS_H
#define MOD_NOTIFICATIONS_H

/*-----------------------------------------------------------------------------
 * Notifications include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Notifications exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_notifications_SetMailFolderPath(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * Notifications module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_notifications_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_NOTIFICATIONS_H											     */
/*---------------------------------------------------------------------------*/
