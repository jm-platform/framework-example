/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_GPRS_MESSAGE_H
#define MOD_GPRS_MESSAGE_H

/*-----------------------------------------------------------------------------
 * GPRS message include files
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * GPRS message define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_GPRS_MESSAGE_CARIAGE_RETURN_STR                     "\r"
#define MOD_GPRS_MESSAGE_END_OF_LINE_STR                        "\n"
#define MOD_GPRS_MESSAGE_END_OK_STR                             "\r\nOK\r\n"
#define MOD_GPRS_MESSAGE_END_ERROR_STR                          "\r\nERROR\r\n"
#define MOD_GPRS_MESSAGE_EMPTY_STR                              ""
#define MOD_GPRS_MESSAGE_SPACE_STR                              " "

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_MESSAGE_OK_STR                                 "OK"
#define MOD_GPRS_MESSAGE_ERROR_STR                              "ERROR"

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_MESSAGE_ARGUMENTS_MAX                          16

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_MESSAGE_ARGUMENTS_SEPERATOR                    ' '
#define MOD_GPRS_MESSAGE_DATA_SEPERATOR                         ','

/*---------------------------------------------------------------------------*/
#define MOD_GPRS_MESSAGE_DATA_CODE_SUCCES                       "0"

/*-----------------------------------------------------------------------------
 * GPRS message type definitions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * GPRS message exposed functions
 *
 *---------------------------------------------------------------------------*/
void mod_gprs_ConvertSerialMessage(String &message);
void mod_gprs_TokenizeSerialMessage(const String &message, 
                                    int &tokenAmount, 
                                    String tokens[], 
                                    char seperator);

/*---------------------------------------------------------------------------*/
#endif /* MOD_GPRS_MESSAGE_H											     */
/*---------------------------------------------------------------------------*/