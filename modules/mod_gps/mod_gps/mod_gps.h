/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_GPS_H
#define MOD_GPS_H

/*-----------------------------------------------------------------------------
 * Gps include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Gps exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_gps_SetSerialPort(SettingsObjectData &settingsObjectData);
int	mod_gps_SetSerialBaudrate(SettingsObjectData &settingsObjectData);
int	mod_gps_SetLocationUpdateInterval(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * Gps module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_gps_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_GPS_H											                 */
/*---------------------------------------------------------------------------*/
