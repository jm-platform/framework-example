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
 * GPRS message include times
 *
 *---------------------------------------------------------------------------*/
#include "mod_gprs_message.h" 

/*-----------------------------------------------------------------------------
 * GPRS message functions
 *
 *---------------------------------------------------------------------------*/
void mod_gprs_ConvertSerialMessage(String &message)
{
	size_t itterator = 0;
	while((itterator = message.find(MOD_GPRS_MESSAGE_END_OK_STR, itterator)) != String::npos) 
	{
		message.replace(itterator, sizeof(MOD_GPRS_MESSAGE_END_OK_STR), 
						String(MOD_GPRS_MESSAGE_OK_STR));

		itterator += sizeof(MOD_GPRS_MESSAGE_OK_STR);
	}

	itterator = 0;
	while((itterator = message.find(MOD_GPRS_MESSAGE_END_ERROR_STR, itterator)) != String::npos) 
	{
		message.replace(itterator, sizeof(MOD_GPRS_MESSAGE_END_ERROR_STR), 
						String(MOD_GPRS_MESSAGE_ERROR_STR));

		itterator += sizeof(MOD_GPRS_MESSAGE_ERROR_STR);
	}

	itterator = 0;
	if ((itterator = message.find(MOD_GPRS_MESSAGE_CARIAGE_RETURN_STR, itterator)) != String::npos) 
	{
		message.replace(itterator, sizeof(MOD_GPRS_MESSAGE_CARIAGE_RETURN_STR), 
						String(MOD_GPRS_MESSAGE_EMPTY_STR));
	}

	itterator = 0;
	while((itterator = message.find(MOD_GPRS_MESSAGE_CARIAGE_RETURN_STR, itterator)) != String::npos) 
	{
		message.replace(itterator, sizeof(MOD_GPRS_MESSAGE_CARIAGE_RETURN_STR), 
						String(1, MOD_GPRS_MESSAGE_ARGUMENTS_SEPERATOR));

		itterator += 1;
	}

	itterator = 0;
	while((itterator = message.find(MOD_GPRS_MESSAGE_END_OF_LINE_STR, itterator)) != String::npos) 
	{
		message.replace(itterator, sizeof(MOD_GPRS_MESSAGE_END_OF_LINE_STR), 
						String(MOD_GPRS_MESSAGE_EMPTY_STR));

		itterator += sizeof(MOD_GPRS_MESSAGE_EMPTY_STR);
	}
}

/*---------------------------------------------------------------------------*/
void mod_gprs_TokenizeSerialMessage(const String &message, 
                                    int &tokenAmount, 
                                    String tokens[], 
                                    char seperator)
{
	String token;
	String previousToken;

    std::istringstream arg(message);
    std::getline(arg, token, seperator);

	tokenAmount = 0;

    previousToken = tokens[tokenAmount++] = token;

    std::getline(arg, token, seperator);

    while (previousToken != token)
    {
        previousToken = tokens[tokenAmount++] = token;

		std::getline(arg, token, seperator);
    }
}

/*---------------------------------------------------------------------------*/