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
 * Synthesizer include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_synthesizer.h"

/*---------------------------------------------------------------------------*/
#include "mod_synthesizer_imp.h"

/*-----------------------------------------------------------------------------
 * Synthesizer settings and workers
 *
 *---------------------------------------------------------------------------*/
extern SynthesizerSettings synthesizerSettings;
extern SynthesizerWorkers  synthesizerWorkers;

/*-----------------------------------------------------------------------------
 * Synthesizer setter function
 *
 *---------------------------------------------------------------------------*/
int	mod_synthesizer_SetSettingsPath(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		synthesizerSettings.settingsPath = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int	mod_synthesizer_SetSoundFontsDirPath(SettingsObjectData &settingsObjectData)
{
	int returnCode = RETURN_CODE_OK;

	if((settingsObjectData.settingsCount == 1) &&
	   (settingsObjectData.settingsValue[0].length() > 0))
	{
		synthesizerSettings.soundFontsDirPath = settingsObjectData.settingsValue[0];

		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "%s%s\"%s\"", settingsObjectData.settingsName.c_str(),
									MOD_DEBUG_MESSAGE_EQUAL_STR, 
									settingsObjectData.settingsValue[0].c_str());
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Synthesizer module interface
 * It is not necessary to implement all data types and functions, simply leave
 * the pointer NULL if you've got nothing to do for the handler.
 *
 *---------------------------------------------------------------------------*/
const ModuleInterface mod_synthesizer_ModuleInterface =
{

		/* application info */
        MOD_SYNTHESIZER_NAME,
		MOD_SYNTHESIZER_VERSION,
		MOD_SYNTHESIZER_BUILD,

		/* Exposed functions of the module */
		mod_synthesizer_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_synthesizer_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_synthesizer_PostInitialisation,				/* Module PostInitialisationPtr */
		mod_synthesizer_DestroyData,					/* Module DestroyDataPtr */
		mod_synthesizer_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    			/* Module TestFunctionPtr */

};

/*---------------------------------------------------------------------------*/