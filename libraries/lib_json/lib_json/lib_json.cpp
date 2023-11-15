/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Lib json include files
 *
 *---------------------------------------------------------------------------*/
#include "lib_json.h" 

/*-----------------------------------------------------------------------------
 * Lib json functions
 *
 *---------------------------------------------------------------------------*/
int lib_json_ReadJsonFile(json_object **object, const String jsonPath)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    *object = json_object_from_file(jsonPath.c_str());

    if (object != NULL)
    {
        if (json_object_object_length(*object) > 0)
        {
            returnCode = RETURN_CODE_OK;
        }
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_json_WriteJsonFile(const String jsonPath, json_object *object)
{
    int returnCode = json_object_to_file_ext(jsonPath.c_str(), object, JSON_C_TO_STRING_PRETTY);

    if (returnCode != RETURN_CODE_FAIL)
    {
        returnCode = RETURN_CODE_OK;
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_json_ReadJsonString(String &data, const String filter, const json_object *object)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    json_object *jsonObject = json_object_object_get(object, filter.c_str());

    if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_string))
    {
        if (json_object_get_string_len(jsonObject) > 0)
        {
            data.assign(json_object_get_string(jsonObject));

            returnCode = RETURN_CODE_OK;
        }
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_json_ReadJsonInteger(int &data, const String filter, const json_object *object)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    json_object *jsonObject = json_object_object_get(object, filter.c_str());

    if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_int))
    {
        data = json_object_get_int(jsonObject);

        returnCode = RETURN_CODE_OK;
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_json_ReadJsonBoolean(bool &data, const String filter, const json_object *object)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    json_object *jsonObject = json_object_object_get(object, filter.c_str());

    if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_boolean))
    {
        data = json_object_get_boolean(jsonObject);

        returnCode = RETURN_CODE_OK;
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
