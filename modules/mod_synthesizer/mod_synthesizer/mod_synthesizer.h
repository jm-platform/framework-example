/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_SYNTHESIZER_H
#define MOD_SYNTHESIZER_H

/*-----------------------------------------------------------------------------
 * Synthesizer include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Synthesizer exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_synthesizer_SetSettingsPath(SettingsObjectData &settingsObjectData);
int	mod_synthesizer_SetSoundFontsDirPath(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * Synthesizer module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_synthesizer_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_SYNTHESIZER_H											         */
/*---------------------------------------------------------------------------*/
