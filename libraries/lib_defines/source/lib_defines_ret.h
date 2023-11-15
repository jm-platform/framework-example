/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_DEFINES_RET_H
#define LIB_DEFINES_RET_H

/*-----------------------------------------------------------------------------
 * Lib defines return codes include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*-----------------------------------------------------------------------------
 * Lib defines return codes 
 *
 *---------------------------------------------------------------------------*/
#define RETURN_CODE_FAIL												-1
#define RETURN_CODE_OK													0

/*---------------------------------------------------------------------------*/
#define RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR						1
#define RETURN_CODE_APPLICATION_CLEAN_EXIT      						2
#define RETURN_CODE_APPLICATION_EXIT_FORCE      						3

/*---------------------------------------------------------------------------*/
#define RETURN_CODE_ERR_UNKNOWN											10

/*---------------------------------------------------------------------------*/
#define RETURN_CODE_ERR_FILE_READ_FAILED								20
#define RETURN_CODE_ERR_DEVICE_UNKNOWN								    21

/*---------------------------------------------------------------------------*/
#endif /* LIB_DEFINES_RET_H											         */
/*---------------------------------------------------------------------------*/