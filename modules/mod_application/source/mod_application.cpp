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
 * application module include files
 *
 *---------------------------------------------------------------------------*/
#include "mod_application.h" 

/*-----------------------------------------------------------------------------
 * Skeleton settings and workers
 *
 *---------------------------------------------------------------------------*/
ApplicationSettings applicationSettings;
ApplicationWorkers 	applicationWorkers;

/*-----------------------------------------------------------------------------
 * Application exit functions
 *
 *---------------------------------------------------------------------------*/
void mod_app_CatchSignal(int signal __attribute__((unused)))
{
	applicationWorkers.applicationContinue = false;
}

/*---------------------------------------------------------------------------*/
void mod_app_CleanExitInit()
{
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = mod_app_CatchSignal;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);
}

/*-----------------------------------------------------------------------------
 * Application process command lines
 *
 *---------------------------------------------------------------------------*/
int mod_app_ProcessCommandLines(const int argc, const char *argv[])
{
	int returnCode = RETURN_CODE_OK;

	for (int argumentCount = 0; argumentCount < argc; argumentCount++)
	{
		String argumentType, argument;

		returnCode = lib_file_GetArgumentContent(argv[argumentCount], argumentType, argument);

		if (returnCode == RETURN_CODE_OK)
		{
			if ((argumentType == MOD_APPLICATION_ARGUMENT_INIFILE_PATH) &&
				(argument.length() != 0))
			{
				applicationSettings.inifilePath = argument;
			}

			else if (argumentType == MOD_APPLICATION_ARGUMENT_DEBUG_MODE)
			{ 
				try
				{
					uint32_t debugMode = std::stoul(argument, nullptr, 16);
					mod_debug_SetDebugMode(debugMode);
				}
				
				catch(const std::exception& e)
				{
					std::cerr << e.what() << ": " << "debug mask invalid" <<'\n';
					return RETURN_CODE_APPLICATION_EXIT_FATAL_ERROR;
				}
			}
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Application post initialization
 *
 *---------------------------------------------------------------------------*/
int mod_app_ReadIniFileSettings()
{
    int returnCode = RETURN_CODE_OK;

	SettingsObjectDataList settingsObjectDatalist;

	settingsObjectDatalist.clear();

	returnCode = lib_file_ReadInifile(applicationSettings.inifilePath, settingsObjectDatalist);

	if (returnCode == RETURN_CODE_OK)
	{
		/* Call the module settings setter functions */
		for (int module = 0; moduleInterfaces[module] != NULL; module++)
		{
			(moduleInterfaces[module]->settingsHandler)(settingsObjectDatalist);
		}
	}

	else if (returnCode == RETURN_CODE_ERR_FILE_READ_FAILED)
	{
		//TODO LOG read failed

		returnCode = RETURN_CODE_OK;
	}

    return returnCode;
}

/*-----------------------------------------------------------------------------
 * Application pre-initialization
 *
 *---------------------------------------------------------------------------*/
int mod_app_PreInitialization()
{
	int returnCode = RETURN_CODE_OK;

    /* Call the module pre-initialization functions */
    for (int module = 0; moduleInterfaces[module] != NULL; module++)
    {
        if (moduleInterfaces[module]->modulePreInitialization != NULL)
        {
            (moduleInterfaces[module]->modulePreInitialization)();
        }
    }

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Application post initialization
 *
 *---------------------------------------------------------------------------*/
int mod_app_PostInitialization()
{
    int returnCode	= RETURN_CODE_OK;

	for (int module = 0; moduleInterfaces[module] != NULL; module++)
	{
		if (moduleInterfaces[module]->modulePostInitialization != NULL)
		{
			returnCode = (moduleInterfaces[module]->modulePostInitialization)();

			if (returnCode != RETURN_CODE_OK)
			{
				break;
			}
		}
	}

    return returnCode;
}


/*-----------------------------------------------------------------------------
 * Application mainloop function
 *
 *---------------------------------------------------------------------------*/
int mod_app_MainloopFunction()
{
	int returnCode = RETURN_CODE_OK;

	for (int module = 0; moduleInterfaces[module] != NULL; module++)
	{
		if (moduleInterfaces[module]->moduleMainloopFunction != NULL)
		{
			returnCode = (moduleInterfaces[module]->moduleMainloopFunction)();

			if (returnCode != RETURN_CODE_OK)
			{
				break;
			}
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Application destroy data function
 *
 *---------------------------------------------------------------------------*/
int mod_app_DestroyData()
{
	int returnCode = RETURN_CODE_OK;

	for (int module = 0; moduleInterfaces[module] != NULL; module++)
	{
		if (moduleInterfaces[module]->moduleDestroyData != NULL)
		{
			returnCode = (moduleInterfaces[module]->moduleDestroyData)();

			if (returnCode != RETURN_CODE_OK)
			{
				break;
			}
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * Application module initializer
 *
 *---------------------------------------------------------------------------*/
void mod_app_Initialization()
{
	/* Initialise the settings */
	applicationSettings.inifilePath = MOD_APPLICATION_INITIAL_INIFILE_PATH;

	/* Initialise the workers */
	applicationWorkers.sleepTimeRequest.tv_sec 	= APPLICATION_PROCESS_SLEEP_INTERVAL / 1000000000;
	applicationWorkers.sleepTimeRequest.tv_nsec = APPLICATION_PROCESS_SLEEP_INTERVAL % 1000000000;

	applicationWorkers.applicationContinue = true;

	mod_app_CleanExitInit();
}

/*-----------------------------------------------------------------------------
 * Application main
 *
 *---------------------------------------------------------------------------*/
int main(const int argc, const char *argv[])
{
	int returnCode = RETURN_CODE_OK;

	try
	{
		mod_app_Initialization();

		returnCode = mod_app_PreInitialization();

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_app_ProcessCommandLines(argc, argv);
        }

        if (returnCode == RETURN_CODE_OK)
		{
            returnCode = mod_app_ReadIniFileSettings();
        }

        if (returnCode == RETURN_CODE_OK)
		{
            returnCode = mod_app_PostInitialization();
        }

		while ((returnCode == RETURN_CODE_OK) &&
			   (applicationWorkers.applicationContinue))
		{
			returnCode = mod_app_MainloopFunction();

			nanosleep(&applicationWorkers.sleepTimeRequest,
						&applicationWorkers.sleepTimeRemain);
		}

		returnCode = mod_app_DestroyData();

		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "Exiting: goodbye");
	}
	catch(const std::exception &ex)
	{
		returnCode = mod_app_DestroyData();
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/