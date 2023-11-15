/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_SYSTEM_H
#define LIB_SYSTEM_H

/*-----------------------------------------------------------------------------
 * Lib system include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include "../../lib_defines/lib_defines.h"
#include "../../application_interface/application_interface.h"

/*-----------------------------------------------------------------------------
 * Lib system define values
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Lib system exposed functions
 *
 *---------------------------------------------------------------------------*/
String lib_system_GetStorageCapacityString();
String lib_system_GetLinuxInfoString();
String lib_system_GePlatformInfoString();

/*---------------------------------------------------------------------------*/
#endif /* LIB_SYSTEM_H											             */
/*---------------------------------------------------------------------------*/