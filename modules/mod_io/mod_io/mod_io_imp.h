/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_IO_IMP_H
#define MOD_IO_IMP_H

/*-----------------------------------------------------------------------------
 * IO include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*---------------------------------------------------------------------------*/
#include <bcm2835.h>

/*-----------------------------------------------------------------------------
 * IO define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_IO_SYSTEM_TYPE_UNKNOWN                          "unknown"
#define MOD_IO_SYSTEM_TYPE_RPI                              "raspberry-pi"

/*---------------------------------------------------------------------------*/
#define MOD_IO_RPI_GPIO_MASK_ALL                            2147483647

/*---------------------------------------------------------------------------*/
#define MOD_IO_INPUT_LOOP_SLEEP_INTERVAL				    20000000	// nano seconds = 20ms

/*-----------------------------------------------------------------------------
 * IO ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_IO_SETTING_SYSTEM_TYPE_STR_LENGTH_MIN	        3
#define MOD_IO_SETTING_SYSTEM_TYPE_STR_LENGTH_MAX	        12

/*---------------------------------------------------------------------------*/
#define MOD_IO_SETTING_RPI_GPIO_MIN	                        2
#define MOD_IO_SETTING_RPI_GPIO_MAX	                        27

/*-----------------------------------------------------------------------------
 * IO initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_IO_INITIAL_SYSTEM_TYPE_STR			            MOD_IO_SYSTEM_TYPE_UNKNOWN

/*-----------------------------------------------------------------------------
 * IO settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_IO_SETTING_SYSTEM_TYPE_STR   			        "io-system-type"

/*-----------------------------------------------------------------------------
 * Lib mqtt type definitions
 *
 *---------------------------------------------------------------------------*/
typedef int InputSetCallback();

/*---------------------------------------------------------------------------*/
typedef struct InputAction
{
    uint8_t             inputPin;
    InputSetCallback*   inputCallback;

} InputAction;

/*---------------------------------------------------------------------------*/
typedef struct OutputAction
{
    int8_t outputPin;
    int8_t outputState;

} OutputAction;

/*---------------------------------------------------------------------------*/
typedef std::list <InputAction> InputList;
typedef std::list <int> InputTriggers;
typedef std::list <OutputAction> OutputTriggers;

/*-----------------------------------------------------------------------------
 * IO settings
 *
 *---------------------------------------------------------------------------*/
typedef struct IOSettings
{
    String  systemType;

} IOSettings;

/*-----------------------------------------------------------------------------
 * IO workers
 *
 *---------------------------------------------------------------------------*/
typedef struct IOWorkers
{
    timer_t         timer1HzId;
    bool            timer1HzTrigger;

    InputList       inputList;
    InputTriggers   inputTriggers;
    bool            inputLoopEnabled;

    uint8_t         inputState[MOD_IO_SETTING_RPI_GPIO_MAX + 1];
    OutputTriggers  outputTriggers;

} IOWorkers;

/*-----------------------------------------------------------------------------
 * IO exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_io_PreInitialisation();
int  mod_io_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_io_PostInitialisation();
int  mod_io_DestroyData();
int  mod_io_MainloopFunction();
int  mod_io_TestFunction();

/*---------------------------------------------------------------------------*/
int mod_io_RegisterInputCallback(const int inputPin, InputSetCallback* inputCallback);
int mod_io_ToggleOutput(const int outputPin);
int mod_io_InitializeInput(const int inputPin);

/*---------------------------------------------------------------------------*/
void mod_io_SetOutputTrigger(const int outputPin, const int outputState);
int  mod_io_GetInputState(const int inputPin);

/*---------------------------------------------------------------------------*/
#endif /* MOD_IO_IMP_H											             */
/*---------------------------------------------------------------------------*/
