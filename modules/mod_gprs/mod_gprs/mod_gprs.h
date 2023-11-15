/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_GPRS_H
#define MOD_GPRS_H

/*-----------------------------------------------------------------------------
 * GPRS include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * GPRS exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_gprs_SetSerialPort(SettingsObjectData &settingsObjectData);
int	mod_gprs_SetSerialBaudrate(SettingsObjectData &settingsObjectData);
int	mod_gprs_SetLocationUpdateInterval(SettingsObjectData &settingsObjectData);
int	mod_gprs_SetStartUpDelay(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * GPRS module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_gprs_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_GPRS_H											             */
/*---------------------------------------------------------------------------*/
