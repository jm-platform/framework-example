/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_SYNTHESIZER_IMP_H
#define MOD_SYNTHESIZER_IMP_H

/*-----------------------------------------------------------------------------
 * Synthesizer include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"
#include <fluidsynth.h>

/*-----------------------------------------------------------------------------
 * Synthesizer define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_NAME_INSTRUMENTS_STR  		    "instruments"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_NAME_NAME_STR  		            "name"
#define MOD_SYNTHESIZER_NAME_SOUND_FONT_STR  		    "soundFont"
#define MOD_SYNTHESIZER_NAME_POLYPHONE_STR  		    "polyphone"
#define MOD_SYNTHESIZER_NAME_KEY_TYPE_STR  		        "keyType"
#define MOD_SYNTHESIZER_NAME_TRIGGER_TYPE_STR           "triggerType"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_NAME_PITCH_START_STR  		    "pitchStart"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_NAME_KEY_STR  		            "key"
#define MOD_SYNTHESIZER_NAME_NOTE_START_STR  		    "noteStart"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_NAME_BPM_MIN_STR  		        "bpmMin"
#define MOD_SYNTHESIZER_NAME_BPM_MAX_STR  		        "bpmMax"
#define MOD_SYNTHESIZER_NAME_BPM_START_STR  		    "bpmStart"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_VALUE_PITCH_STR  		        "pitch"
#define MOD_SYNTHESIZER_VALUE_KEY_STR  		            "key"
#define MOD_SYNTHESIZER_VALUE_NOTES_STR  		        "notes"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_VALUE_AUTO_STR  		        "auto"
#define MOD_SYNTHESIZER_VALUE_TRIGGER_STR  		        "trigger"
#define MOD_SYNTHESIZER_VALUE_REPEATING_STR  		    "repeating"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_VELOCITY_MAX  		            127

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_AUDIO_DRIVER  		            "audio.driver"
#define MOD_SYNTHESIZER_AUDIO_DRIVER_ALSA  		        "alsa"

/*---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_PITCH_BEND_MAX  		        16383
#define MOD_SYNTHESIZER_NOTE_MAX  		                127
#define MOD_SYNTHESIZER_PITCH_SENSITIVITY  		        500

/*-----------------------------------------------------------------------------
 * Synthesizer ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX           6

/*-----------------------------------------------------------------------------
 * Synthesizer initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_INITIAL_SETTINGS_PATH_STR       "../data/synth-settings.json"
#define MOD_SYNTHESIZER_INITIAL_SF_DIR_PATH_STR         "../data/soundFonts/"

/*-----------------------------------------------------------------------------
 * Synthesizer settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_SYNTHESIZER_SETTING_SET_SETTINGS_PATH_STR   "synthesizer-settings-path"
#define MOD_SYNTHESIZER_SETTING_SF_DIR_PATH_STR         "synthesizer-sound-fonts-dir-path"

/*-----------------------------------------------------------------------------
 * Synthesizer type definitions
 *
 *---------------------------------------------------------------------------*/
typedef enum KeyType
{
    keyTypePitch = 0,
    keyTypeNotes,
    keyTypeKey,
    keyTypeUnknown = -1

} KeyType;

/*---------------------------------------------------------------------------*/
typedef enum TriggerType
{
    triggerTypeAuto = 0,
    triggerTypeTrigger,
    triggerTypeRepeating,
    triggerTypeUnknown = -1

} TriggerType;

/*---------------------------------------------------------------------------*/
typedef struct Instrument
{
    bool            enabled;
    
    fluid_synth_t*  synth;

    String          name;
    String          soundFont;
    bool            poly;
    
    KeyType         keyType;
    TriggerType     triggerType;

    int             pitchStart;
    String          noteStart;
    String          musicKey;

    int             bpmMin;
    int             bpmMax;
    int             bpmStart;

    int             currentNote;
    int             currentPitch;
    int             currentBpm;

    int             bpmTimer;
    int             bpmTimerEnd;

    int             velocity;

    int             nextNote;

} Instrument;

/*-----------------------------------------------------------------------------
 * Synthesizer settings
 *
 *---------------------------------------------------------------------------*/
typedef struct SynthesizerSettings
{
    String      settingsPath;
    String      soundFontsDirPath;

} SynthesizerSettings;

/*-----------------------------------------------------------------------------
 * Synthesizer workers
 *
 *---------------------------------------------------------------------------*/
typedef struct SynthesizerWorkers
{
    timer_t                 timer1HzId;
    bool                    timer1HzTrigger;

    fluid_settings_t*       settings;
    fluid_audio_driver_t*   adriver;

    bool                    synthEnabled;

    fluid_synth_t*          synth;
    int                     currentNote;

    Instrument              instrument[MOD_SYNTHESIZER_INSTRUMENT_AMOUNT_MAX];

} SynthesizerWorkers;

/*-----------------------------------------------------------------------------
 * Synthesizer exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_synthesizer_PreInitialisation();
int  mod_synthesizer_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_synthesizer_PostInitialisation();
int  mod_synthesizer_DestroyData();
int  mod_synthesizer_MainloopFunction();
int  mod_synthesizer_TestFunction();

/*---------------------------------------------------------------------------*/
void mod_synthesizer_InstrumentPitch(const int &instrument, const int &pitchAmount);
void mod_synthesizer_InstrumentSetVelocity(const int &instrument, const int &bpmChange);
void mod_synthesizer_InstrumentTrigger(const int &instrument);
void mod_synthesizer_InstrumentSetBpm(const int &instrument, const int &bpmChange);

/*---------------------------------------------------------------------------*/
#endif /* MOD_SYNTHESIZER_IMP_H											     */
/*---------------------------------------------------------------------------*/
