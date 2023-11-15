/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_DEBUG_H
#define MOD_DEBUG_H

/*-----------------------------------------------------------------------------
 * Debug include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Debug module exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_debug_SetLoggingMode(SettingsObjectData &settingsObjectData);
int	mod_debug_SetLogDirectory(SettingsObjectData &settingsObjectData);
int	mod_debug_SetChannelEnabled(SettingsObjectData &settingsObjectData);
int	mod_debug_SetChannelTimeout(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * Debug module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_debug_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_DEBUG_H											             */
/*---------------------------------------------------------------------------*/
