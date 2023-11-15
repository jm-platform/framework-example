/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_PUPPET_DEVICE_H
#define MOD_PUPPET_DEVICE_H

/*-----------------------------------------------------------------------------
 * Skeleton include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Skeleton exposed setter functions
 *
 *---------------------------------------------------------------------------*/
int	mod_puppet_device_SetSensorInfoPath(SettingsObjectData &SettingsObjectData);
int	mod_puppet_device_SetPrintNRFDetails(SettingsObjectData &settingsObjectData);
int	mod_puppet_device_SetPuppetDeviceRole(SettingsObjectData &settingsObjectData);
int	mod_puppet_device_SetRFChannel(SettingsObjectData &settingsObjectData);
int	mod_puppet_device_SetRFPowerLevel(SettingsObjectData &settingsObjectData);
int	mod_puppet_device_SetTimeout(SettingsObjectData &settingsObjectData);
int	mod_puppet_device_SetEnableRF(SettingsObjectData &settingsObjectData);
int	mod_puppet_device_SetNRFMode(SettingsObjectData &settingsObjectData);

/*-----------------------------------------------------------------------------
 * Skeleton module inface declararion
 *
 *---------------------------------------------------------------------------*/
extern const ModuleInterface mod_puppet_device_ModuleInterface;

/*---------------------------------------------------------------------------*/
#endif /* MOD_PUPPET_DEVICE_H											     */
/*---------------------------------------------------------------------------*/
