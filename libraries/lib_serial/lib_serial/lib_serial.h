/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_SERIAL_H
#define LIB_SERIAL_H

/*-----------------------------------------------------------------------------
 * Lib Serial include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include "../../lib_defines/lib_defines.h"

/*---------------------------------------------------------------------------*/
#include "../../../modules/mod_debug/mod_debug.h"

/*---------------------------------------------------------------------------*/
#include <termios.h>

/*-----------------------------------------------------------------------------
 * Lib Serial define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_SERIAL_MAX_UART_MESSAGE_LENGTH                 		256

/*-----------------------------------------------------------------------------
 * Lib Serial type definition
 *
 *---------------------------------------------------------------------------*/
typedef int Connection;

/*-----------------------------------------------------------------------------
 * Lib Serial exposed functions
 *
 *---------------------------------------------------------------------------*/
int lib_serial_OpenConnection(Connection &connection, 
							  const String &address,
							  const int &baudrate,
							  bool setFlags = true);
int lib_serial_ReadFromConnection(Connection &connection, 
							  	  int &messageLength,
							  	  String &message);

/*---------------------------------------------------------------------------*/
#endif /* LIB_SERIAL_H											         	 */
/*---------------------------------------------------------------------------*/