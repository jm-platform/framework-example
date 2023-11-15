/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_DEBUG_IMP_H
#define MOD_DEBUG_IMP_H

/*-----------------------------------------------------------------------------
 * Debug include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"


/*-----------------------------------------------------------------------------
 * Debug define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_DEBUG_LOG_MODE_TYPE_NONE	                    0x00000000
#define MOD_DEBUG_LOG_MODE_TYPE_ERROR	                    0x00000001
#define MOD_DEBUG_LOG_MODE_TYPE_GENERAL	                    0x00000002
#define MOD_DEBUG_LOG_MODE_TYPE_MESSAGING                   0x00000010
#define MOD_DEBUG_LOG_MODE_TYPE_STATEMACHINE                0x00000100

/*---------------------------------------------------------------------------*/
#define MOD_DEBUG_MESSAGE_TAB_STR	                        "\t"
#define MOD_DEBUG_MESSAGE_KEY_STR	                        "message="
#define MOD_DEBUG_MESSAGE_EQUAL_STR	                        "="
#define MOD_DEBUG_MESSAGE_END_OF_LINE_STR	                "\n"

/*---------------------------------------------------------------------------*/
#define MOD_DEBUG_MESSAGE_LENGTH_MAX	                    255

/*---------------------------------------------------------------------------*/
#define MOD_DEBUG_MESSAGE_FILE_PREFIX_STR	                "jmp-log-"
#define MOD_DEBUG_MESSAGE_FILE_SUFIX_STR	                ".log"
#define MOD_DEBUG_MESSAGE_FILE_HEADER_STR	                "jm-platform log "

/*---------------------------------------------------------------------------*/
#define MOD_DEBUG_CHANNEL_START	                            "start"
#define MOD_DEBUG_CHANNEL_PING	                            "ping"
#define MOD_DEBUG_CHANNEL_STOP	                            "stop"

/*-----------------------------------------------------------------------------
 * Debug ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_DEBUG_SETTING_LOG_MODE_STR_LENGTH_MIN	        3
#define MOD_DEBUG_SETTING_LOG_MODE_STR_LENGTH_MAX	        11

/*---------------------------------------------------------------------------*/
#define MOD_DEBUG_SETTING_LOG_DIRECTORY_STR_LENGTH_MAX	    255

/*---------------------------------------------------------------------------*/
#define MOD_DEBUG_SETTING_CHANNEL_TIMEOUT_MIN	            2
#define MOD_DEBUG_SETTING_CHANNEL_TIMEOUT_MAX	            3600

/*-----------------------------------------------------------------------------
 * Debug initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_DEBUG_INITIAL_LOG_MODE_STR   			        2
#define MOD_DEBUG_INITIAL_LOG_DIRECTORY_STR			        "../log/"
#define MOD_DEBUG_INITIAL_CHANNEL_ENABLED_STR			    false
#define MOD_DEBUG_INITIAL_CHANNEL_TIMEOUT_STR			    5

/*-----------------------------------------------------------------------------
 * Debug settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_DEBUG_SETTING_LOG_MODE_STR   			        "debug-log-mode"
#define MOD_DEBUG_SETTING_LOG_DIRECTORY_STR			        "debug-log-directory"
#define MOD_DEBUG_SETTING_CHANNEL_ENABLED_STR			    "debug-channel-enabled"
#define MOD_DEBUG_SETTING_CHANNEL_TIMEOUT_STR		        "debug-channel-timeout"

/*-----------------------------------------------------------------------------
 * Define macro log and debug functions
 *---------------------------------------------------------------------------*/
#define debug_masked_log(mask,msg)		        {LogInfo log = { __FUNCTION__, (msg)};	\
										        mod_debug_debugLog((mask), &log);}
#define debug_masked_printf(mask,format,...)    mod_debug_debugPrintf(mask, __FUNCTION__, format, ##__VA_ARGS__);

/*-----------------------------------------------------------------------------
 * Debug type definitions
 *
 *---------------------------------------------------------------------------*/
typedef uint32_t DebugLogMode;

/*-----------------------------------------------------------------------------
 * Debug information struct
 *---------------------------------------------------------------------------*/
typedef struct LogInfo
{
	String  function;
	String  message;

} LogInfo;

/*-----------------------------------------------------------------------------
 * Debug message struct
 *---------------------------------------------------------------------------*/
typedef struct LogMessage
{
    uint32_t    mask;
	LogInfo     logInfo;

} LogMessage;

/*---------------------------------------------------------------------------*/
typedef std::list<LogMessage> LogMessageList;

/*-----------------------------------------------------------------------------
 * Debug settings
 *
 *---------------------------------------------------------------------------*/
typedef struct DebugSettings
{
    DebugLogMode    logMode; 
    String          logDirectory;
    
    bool            channelEnabled;
    int             channelTimeout;

} DebugSettings;

/*-----------------------------------------------------------------------------
 * Debug workers
 *
 *---------------------------------------------------------------------------*/
typedef struct DebugWorkers
{
    timer_t         timer1HzId;
    bool            timer1HzTrigger;

    DebugLogMode    debugMode;
    LogMessageList  logMessageList;
    bool            initialized;

    bool            channelStarted;
    int             channelTimer;

} DebugWorkers;

/*-----------------------------------------------------------------------------
 * Debug exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_debug_PreInitialisation();
int  mod_debug_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_debug_PostInitialisation();
int  mod_debug_DestroyData();
int  mod_debug_MainloopFunction();
int  mod_debug_TestFunction();

/*---------------------------------------------------------------------------*/
void mod_debug_SetDebugMode(DebugLogMode debugMode);
void mod_debug_debugLog(uint32_t mask, LogInfo *log);
void mod_debug_debugPrintf(uint32_t mask, const char *funct, const char* format, ...);

/*---------------------------------------------------------------------------*/
#endif /* MOD_DEBUG_IMP_H											         */
/*---------------------------------------------------------------------------*/
