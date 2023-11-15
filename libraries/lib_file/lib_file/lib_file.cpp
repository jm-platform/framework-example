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
 * Lib file include files
 *
 *---------------------------------------------------------------------------*/
#include "lib_file.h" 

/*-----------------------------------------------------------------------------
 * Lib file functions
 *
 *---------------------------------------------------------------------------*/
int lib_file_ReadInifile(const String inifile, SettingsObjectDataList &settingsObjectDataList)
{
    int returnCode = RETURN_CODE_OK;

	std::ifstream file(inifile.c_str());
	String token, argValue, value;
	 
    if (file.is_open())
	{
		while(std::getline(file, token)) 
		{
            //token.erase(std::remove(token.begin(), token.end(), '\n'), token.end());
			std::stringstream line(token);
            SettingsObjectData settingsObjectData;

            int asciiValue = (int)token.at(LIB_FILE_INI_FIRST_CHARACTER);

            /* Check if line is not a comment */
            if ((asciiValue >= LIB_FILE_INI_CHARACTER_RANGE_MIN) &&
                (asciiValue <= LIB_FILE_INI_CHARACTER_RANGE_MAX))
            {
                uint8_t argumentCount = 0;

                std::getline(line, argValue, LIB_FILE_CHARACTER_EQUAL);

                settingsObjectData.settingsName = argValue;

                while (std::getline(line, argValue, LIB_FILE_CHARACTER_COMMA))
                {
                    settingsObjectData.settingsValue[argumentCount++] = argValue;
                    
                }

                settingsObjectData.settingsValue[argumentCount-1].pop_back();
                settingsObjectData.settingsCount = argumentCount;
                settingsObjectDataList.push_back(settingsObjectData);
            }
		}

		file.close();
	}

	else
	{
		returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;
	}

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_GetArgumentContent(const char* source, String &ArgumentType, String &Argument)
{
    int returnCode = RETURN_CODE_FAIL;

    String argValue;

    std::istringstream arg(source);
    std::getline(arg, argValue, LIB_FILE_CHARACTER_EQUAL);

    ArgumentType = argValue;

    std::getline(arg, argValue);

    if (ArgumentType != argValue)
    {
        Argument = argValue;

        returnCode = RETURN_CODE_OK;
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_CheckFile(const String filePath)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    std::ifstream file(filePath.c_str());

    if (file.is_open())
	{
        returnCode = RETURN_CODE_OK;
        file.close();
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_CreateFile(const String filePath)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    std::ofstream file(filePath.c_str());
    file.close();

    returnCode = lib_file_CheckFile(filePath);

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_DeleteFile(const String filePath)
{
    int returnCode = lib_file_CheckFile(filePath);

    if (returnCode == RETURN_CODE_OK)
    {
        std::remove(filePath.c_str());

        if (lib_file_CheckFile(filePath) == RETURN_CODE_ERR_FILE_READ_FAILED)
        {
            returnCode = RETURN_CODE_OK;
        }

        else
        {
            returnCode = RETURN_CODE_FAIL;
        }
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_AppendToFile(const String filePath, const String message)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    std::ofstream file(filePath.c_str(), std::ios_base::app);

    returnCode = lib_file_CheckFile(filePath);

    if ((returnCode == RETURN_CODE_OK) && (file.is_open()))
    {
        file << message;
		file.close();	
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_WriteToFile(const String filePath, const String message)
{
    int returnCode = RETURN_CODE_ERR_FILE_READ_FAILED;

    std::ofstream file(filePath.c_str());

    returnCode = lib_file_CheckFile(filePath);

    if ((returnCode == RETURN_CODE_OK) && (file.is_open()))
    {
        file << message;
		file.close();	
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_file_TokenizeString(const String &message, 
                            int &tokenAmount, 
                            String tokens[],
                            int arrarySize, 
                            char seperator)
{
    int returnCode = RETURN_CODE_OK;

	String token;
    std::istringstream arg(message);

    tokenAmount = 0;

    while (std::getline(arg, token, seperator))
    {
        if ((arrarySize - 1) < tokenAmount)
        {
            returnCode = RETURN_CODE_FAIL;
            break;
        }

        else
        {
            tokens[tokenAmount++] = token;
        }
    }

    return returnCode;
}

/*---------------------------------------------------------------------------*/