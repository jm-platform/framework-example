/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_FILE_H
#define LIB_FILE_H

/*-----------------------------------------------------------------------------
 * Lib file include files
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"

/*---------------------------------------------------------------------------*/
#include "../../application_interface/application_interface.h"
#include "../../lib_defines/lib_defines.h"


/*-----------------------------------------------------------------------------
 * Lib file define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_FILE_INI_FIRST_CHARACTER                            0

/*---------------------------------------------------------------------------*/
#define LIB_FILE_INI_CHARACTER_RANGE_MIN                        97
#define LIB_FILE_INI_CHARACTER_RANGE_MAX                        122

/*---------------------------------------------------------------------------*/
#define LIB_FILE_CHARACTER_EQUAL                                '='
#define LIB_FILE_CHARACTER_COMMA                                ','
#define LIB_FILE_CHARACTER_DASH                                 '/'

/*-----------------------------------------------------------------------------
 * Lib file exposed functions
 *
 *---------------------------------------------------------------------------*/
int lib_file_ReadInifile(const String inifile, SettingsObjectDataList &settingsObjectData);
int lib_file_GetArgumentContent(const char* source, String &ArgumentType, String &Argument);
int lib_file_CheckFile(const String filePath);
int lib_file_CreateFile(const String filePath);
int lib_file_DeleteFile(const String filePath);
int lib_file_AppendToFile(const String filePath, const String message);
int lib_file_WriteToFile(const String filePath, const String message);
int lib_file_TokenizeString(const String &message, 
                            int &tokenAmount, 
                            String tokens[], 
                            int arrarySize, 
                            char seperator);

/*---------------------------------------------------------------------------*/
#endif /* LIB_FILE_H											             */
/*---------------------------------------------------------------------------*/