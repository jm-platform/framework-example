/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_SKELETON_IMP_H
#define MOD_SKELETON_IMP_H

/*-----------------------------------------------------------------------------
 * Skeleton include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Skeleton define values
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Skeleton ranges
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Skeleton initial settings
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Skeleton settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_SKELETON_SETTING_TEST_STR   			"skeleton-module-test"

/*-----------------------------------------------------------------------------
 * Skeleton settings
 *
 *---------------------------------------------------------------------------*/
typedef struct SkeletModSettings
{

} SkeletModSettings;

/*-----------------------------------------------------------------------------
 * Skeleton workers
 *
 *---------------------------------------------------------------------------*/
typedef struct SkeletModWorkers
{
    timer_t timer1HzId;
    bool    timer1HzTrigger;

} SkeletModWorkers;

/*-----------------------------------------------------------------------------
 * Skeleton exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_skeleton_PreInitialisation();
int  mod_skeleton_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_skeleton_PostInitialisation();
int  mod_skeleton_DestroyData();
int  mod_skeleton_MainloopFunction();
int  mod_skeleton_TestFunction();

/*---------------------------------------------------------------------------*/
#endif /* MOD_SKELETON_IMP_H											     */
/*---------------------------------------------------------------------------*/
