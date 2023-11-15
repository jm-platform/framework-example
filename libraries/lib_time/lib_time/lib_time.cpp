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
 * Lib time include times
 *
 *---------------------------------------------------------------------------*/
#include "lib_time.h" 

/*-----------------------------------------------------------------------------
 * Lib time functions
 *
 *---------------------------------------------------------------------------*/
String lib_time_GetDateString()
{
	time_t 		rawtime;
	struct tm* 	timeinfo;
	char 		buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"%d-%m-%Y",timeinfo);
	String str(buffer);

	return str;
}

/*---------------------------------------------------------------------------*/
String lib_time_GetDateTimeString()
{
	time_t 		rawtime;
	struct tm* 	timeinfo;
	char 		buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
	String str(buffer);

	return str;
}

/*---------------------------------------------------------------------------*/
String lib_time_GetDateTimeStringISO()
{
	time_t 		rawtime;
	struct tm* 	timeinfo;
	char 		buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"%Y-%m-%dT%H:%M:%S",timeinfo);
	String str(buffer);

	return str;
}


/*---------------------------------------------------------------------------*/
bool lib_time_SetupAndStartTimer(int sig, uint16_t milliSecs, void *handlerFunction, timer_t *timerId)
{
	struct sigevent sigEvent;
	struct itimerspec iTimerSpec;
	struct sigaction sigAction;
	long long nanoSecs;

	// Setup signal handler
	sigAction.sa_flags = 0;
	sigAction.sa_sigaction = NULL;
	sigAction.sa_handler = (void(*)(int))handlerFunction;
	sigemptyset(&sigAction.sa_mask);

	if (sigaction(sig, &sigAction, NULL) == -1)
	{
		return false;
	}

	// Create the signal event timer
	sigEvent.sigev_notify = SIGEV_SIGNAL;
	sigEvent.sigev_signo = sig;
	sigEvent.sigev_value.sival_ptr = timerId;

	if (timer_create(CLOCK_REALTIME, &sigEvent, timerId) == -1)
	{
		return false;
	}

	// Setup and start the timer
	nanoSecs = ((long long)milliSecs * 1000000);
	iTimerSpec.it_value.tv_sec = (nanoSecs / 1000000000);
	iTimerSpec.it_value.tv_nsec = (nanoSecs % 1000000000);
	iTimerSpec.it_interval.tv_sec = iTimerSpec.it_value.tv_sec;
	iTimerSpec.it_interval.tv_nsec = iTimerSpec.it_value.tv_nsec;

	if (timer_settime(*timerId, 0, &iTimerSpec, NULL) == -1)
	{
		return false;
	}

	return true;
}

/*---------------------------------------------------------------------------*/
void lib_time_StopTimer(timer_t timerId)
{
	struct itimerspec iTimerSpec;

	memset((void*)&iTimerSpec, 0, sizeof(iTimerSpec));

	if (timer_settime(timerId, 0, &iTimerSpec, NULL) != -1)
	{
		timer_delete(timerId);
	}
}

/*---------------------------------------------------------------------------*/
