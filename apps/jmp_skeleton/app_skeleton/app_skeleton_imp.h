/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef APP_SKELETON_IMP_H
#define APP_SKELETON_IMP_H

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
#define APP_SKELETON_SETTING_TEST_STR   			"skeleton-test"

/*-----------------------------------------------------------------------------
 * Skeleton settings
 *
 *---------------------------------------------------------------------------*/
typedef struct SkeletonSettings
{

} SkeletonSettings;

/*-----------------------------------------------------------------------------
 * Skeleton workers
 *
 *---------------------------------------------------------------------------*/
typedef struct SkeletonWorkers
{
    timer_t timer1HzId;
    bool    timer1HzTrigger;

} SkeletonWorkers;

/*-----------------------------------------------------------------------------
 * Skeleton exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void app_skeleton_PreInitialisation();
int  app_skeleton_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  app_skeleton_PostInitialisation();
int  app_skeleton_DestroyData();
int  app_skeleton_MainloopFunction();
int  app_skeleton_TestFunction();

/*---------------------------------------------------------------------------*/
#endif /* APP_SKELETON_IMP_H											     */
/*---------------------------------------------------------------------------*/
