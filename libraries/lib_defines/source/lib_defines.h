/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_DEFINES_H
#define LIB_DEFINES_H

/*-----------------------------------------------------------------------------
 * Lib defines include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*-----------------------------------------------------------------------------
 * Lib defines define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_DEFINE_STATEMENT_ON                             "on"
#define LIB_DEFINE_STATEMENT_OFF                            "off"

/*---------------------------------------------------------------------------*/
#define LIB_DEFINE_STATEMENT_TRUE_STR                       "true"
#define LIB_DEFINE_STATEMENT_FALSE_STR                      "false"

/*---------------------------------------------------------------------------*/
#define LIB_DEFINE_STATUS_OK_STR                       		"ok"
#define LIB_DEFINE_STATUS_WARNING_STR                       "warning"
#define LIB_DEFINE_STATUS_ERROR_STR                       	"error"

/*---------------------------------------------------------------------------*/
#define LIB_DEFINE_VALUE_TYPE_STATUS_STR                 	"status"
#define LIB_DEFINE_VALUE_TYPE_DATA_STR                  	"data"
#define LIB_DEFINE_VALUE_UNIT_BOOL_STR                  	"bool"
#define LIB_DEFINE_VALUE_UNIT_STRING_STR                  	"string"
#define LIB_DEFINE_VALUE_UNIT_FLOAT_STR                  	"float"
#define LIB_DEFINE_VALUE_UNIT_INT_STR                  		"int"

/*-----------------------------------------------------------------------------
 * Lib defines Type definitions
 *
 *---------------------------------------------------------------------------*/
typedef std::string String;

/*---------------------------------------------------------------------------*/
#endif /* LIB_DEFINES_H											             */
/*---------------------------------------------------------------------------*/