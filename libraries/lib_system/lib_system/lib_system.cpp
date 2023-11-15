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
 * Lib system include files
 *
 *---------------------------------------------------------------------------*/
#include "lib_system.h"

/*---------------------------------------------------------------------------*/ 
#include <filesystem>
#include <sys/utsname.h>

/*-----------------------------------------------------------------------------
 * Lib system functions
 *
 *---------------------------------------------------------------------------*/
String lib_system_GetStorageCapacityString()
{
	double capacity	 = 0.0;
	double available = 0.0;
	String sizeUnit;
	std::ostringstream messageStream;

	std::filesystem::space_info storage = std::filesystem::space("/tmp");

	if (storage.capacity >= 1000000000)
	{
		capacity  = (storage.capacity / 1000000000.0);
		available = (storage.available / 1000000000.0);
		sizeUnit  = "GB";

		messageStream << std::setprecision(4);
	}

	else
	{
		capacity  = (storage.capacity / 1000000.0);
		available = (storage.available / 1000000.0);
		sizeUnit  = "MB";

		messageStream << std::setprecision(7);
	}
	
	messageStream << "Storage available: " << available << "/" << capacity << sizeUnit;
	String message = String(messageStream.str());

	return message;
}

/*---------------------------------------------------------------------------*/
String lib_system_GetLinuxInfoString()
{
	struct utsname systemInfo;
	std::ostringstream messageStream;
	String message = "system info unknown";

	if (uname(&systemInfo) != -1) 
	{
		messageStream << systemInfo.sysname << " " << systemInfo.machine << ": " << systemInfo.version;
		message = String(messageStream.str());
	}

	return message;
}

/*---------------------------------------------------------------------------*/
String lib_system_GePlatformInfoString()
{
	String info = "JMP-modules: ";

	for (int module = 0; moduleInterfaces[module] != NULL; module++)
    {
		std::ostringstream messageStream;
		messageStream << moduleInterfaces[module]->moduleName << ' ' << 
						 moduleInterfaces[module]->moduleVersion << '.' <<
						 moduleInterfaces[module]->moduleBuild << ", ";

		info = info + String(messageStream.str());
    }

	info = info.substr(0, (info.size()-2));

	return info;
}

/*---------------------------------------------------------------------------*/