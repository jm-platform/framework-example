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
 * Lib Serial include files
 *
 *---------------------------------------------------------------------------*/
#include "lib_serial.h" 

/*-----------------------------------------------------------------------------
 * Lib Serial functions
 *
 *---------------------------------------------------------------------------*/
int lib_serial_ConvertBaudrate(const int &baudrate, int &termiosBaudrate)
{
	int returnCode = RETURN_CODE_OK;

	switch (baudrate)
	{
		case 9600:
			termiosBaudrate = B9600;
			break;
		case 19200:
			termiosBaudrate = B19200;
			break;
		case 57600:
			termiosBaudrate = B57600;
			break;
		case 115200:
			termiosBaudrate = B115200;
			break;
		default:
			returnCode = RETURN_CODE_FAIL;
			break;

	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_serial_OpenConnection(Connection &connection, 
							  const String &address,
							  const int &baudrate,
							  bool setFlags)
{
	int returnCode = RETURN_CODE_OK;

	connection = open(address.c_str(), O_RDWR);

	struct termios tty;

	if(tcgetattr(connection, &tty) != 0) 
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "ERROR: serial failed code: \'%i\' from tcgetattr: \"%s\"", errno, strerror(errno));
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);

		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		if (setFlags)
		{
			tty.c_cflag &= ~PARENB;
			tty.c_cflag &= ~CSTOPB;
			tty.c_cflag &= ~CSIZE;
			tty.c_cflag |= CS8;
			tty.c_cflag &= ~CRTSCTS;
			tty.c_cflag |= CREAD | CLOCAL;

			tty.c_lflag &= ~ICANON;
			tty.c_lflag &= ~ECHO;
			tty.c_lflag &= ~ECHOE;
			tty.c_lflag &= ~ECHONL;
			tty.c_lflag &= ~ISIG;
			tty.c_iflag &= ~(IXON | IXOFF | IXANY); 
			tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

			tty.c_cflag &= ~IGNPAR;

			tty.c_oflag &= ~OPOST; 
			tty.c_oflag &= ~ONLCR;
			
			tty.c_cc[VTIME] = 10; 
			tty.c_cc[VMIN] = 0;
		}

		int termiosBaudrate = 0;

		returnCode = lib_serial_ConvertBaudrate(baudrate, termiosBaudrate);

		if (returnCode == RETURN_CODE_OK)
		{
			cfsetispeed(&tty, termiosBaudrate);
			cfsetospeed(&tty, termiosBaudrate);

			if (tcsetattr(connection, TCSANOW, &tty) != 0) 
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "ERROR: serial failed code: \'%i\' from tcgetattr: \"%s\"", errno, strerror(errno));
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);

				return RETURN_CODE_FAIL;
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int lib_serial_ReadFromConnection(Connection &connection, 
							  	  int &messageLength,
							  	  String &message)
{
	int returnCode = RETURN_CODE_OK;

	char messageBuffer[2000];
	memset(&messageBuffer, '\0', sizeof(messageBuffer));

	messageLength = read(connection, &messageBuffer, sizeof(messageBuffer));

	if ((messageLength < 0) && (errno != EINTR))
	{
		char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
		sprintf(buffer, "serial read error: %s", strerror(errno));
		debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_ERROR, buffer);
	}

	else if (messageLength > 0)
	{
		message = String(messageBuffer);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
