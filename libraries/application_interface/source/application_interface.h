/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef APPLICATION_INTERFACE_H
#define APPLICATION_INTERFACE_H

/*-----------------------------------------------------------------------------
 * This file defines the application module interface.
 *
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Application interface include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include "../../lib_defines/lib_defines.h"
/*-----------------------------------------------------------------------------
 * Application interface define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_APP_INTERFACE_SETTINGS_VALUE_COUNT_MAX                  255


/*-----------------------------------------------------------------------------
 * Type defines of message handlers, functions and data structures
 *
 *---------------------------------------------------------------------------*/
typedef const char* ModuleName;
typedef uint16_t	ModuleVersion;
typedef uint16_t	ModuleBuild;

/*---------------------------------------------------------------------------*/
typedef String      SettingsObjectName;

/*---------------------------------------------------------------------------*/
typedef uint8_t     SettingsObjectDataCount;
typedef String      SettingsObjectDataValue;

/*-----------------------------------------------------------------------------
 * Module prototype function typedefs
 *
 *---------------------------------------------------------------------------*/
typedef void ModulePreInitialization();
typedef int  ModulePostInitialization();
typedef int  ModuleDestroyData();
typedef int  ModuleMainloopFunction();
typedef int  ModuleTestFunction();

/*---------------------------------------------------------------------------*/
typedef struct SettingsObjectData
{
	SettingsObjectName    	    settingsName;
	SettingsObjectDataCount     settingsCount;
    SettingsObjectDataValue     settingsValue[LIB_APP_INTERFACE_SETTINGS_VALUE_COUNT_MAX];

} SettingsObjectData;

/*---------------------------------------------------------------------------*/
typedef std::list<SettingsObjectData> SettingsObjectDataList;

/*---------------------------------------------------------------------------*/
typedef int  SettingsObjectFunction(SettingsObjectData&);
typedef int  SettingsHandler(SettingsObjectDataList&);

/*---------------------------------------------------------------------------*/
typedef struct SettingsObjectTableObject
{
	SettingsObjectName    	    settingsName;
	SettingsObjectFunction      *settingsSetter;

} SettingsObjectTableObject;

/*---------------------------------------------------------------------------*/
typedef struct ModuleInterface
{
    ModuleName	                moduleName;
    ModuleVersion               moduleVersion;
    ModuleBuild	                moduleBuild;

    ModulePreInitialization	    *modulePreInitialization;
    SettingsHandler             *settingsHandler;
    ModulePostInitialization    *modulePostInitialization;
    ModuleDestroyData		    *moduleDestroyData;
    ModuleMainloopFunction	    *moduleMainloopFunction;
    ModuleTestFunction          *moduleTestFunction;

} ModuleInterface;

/*-----------------------------------------------------------------------------
 * The one and only application module interfaces variable declaration
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface *moduleInterfaces[];

/*---------------------------------------------------------------------------*/
#endif /* APPLICATION_INTERFACE_H											 */
/*---------------------------------------------------------------------------*/
