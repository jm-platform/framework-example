/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_JSON_H
#define LIB_JSON_H

/*-----------------------------------------------------------------------------
 * Lib json include jsons
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"
#include <json-c/json.h>

/*---------------------------------------------------------------------------*/
#include "../../application_interface/application_interface.h"
#include "../../lib_defines/lib_defines.h"


/*-----------------------------------------------------------------------------
 * Lib json define values
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Lib json exposed functions
 *
 *---------------------------------------------------------------------------*/
int lib_json_ReadJsonFile(json_object **object, const String jsonPath);
int lib_json_WriteJsonFile(const String jsonPath, json_object *object);

/*---------------------------------------------------------------------------*/
int lib_json_ReadJsonString(String &data, const String filter, const json_object *object);
int lib_json_ReadJsonInteger(int &data, const String filter, const json_object *object);
int lib_json_ReadJsonBoolean(bool &data, const String filter, const json_object *object);

/*---------------------------------------------------------------------------*/
#endif /* LIB_JSON_H											             */
/*---------------------------------------------------------------------------*/