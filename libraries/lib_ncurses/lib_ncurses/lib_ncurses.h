/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_NCURSES_H
#define LIB_NCURSES_H

/*-----------------------------------------------------------------------------
 * Lib ncurses include ncurses
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"
#include <ncurses.h>

/*---------------------------------------------------------------------------*/
#include "../../application_interface/application_interface.h"
#include "../../lib_defines/lib_defines.h"

/*-----------------------------------------------------------------------------
 * Lib ncurses define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_NCURSES_COLOR_BLACK           					"black"
#define LIB_NCURSES_COLOR_WHITE           					"white"
#define LIB_NCURSES_COLOR_RED             					"red"
#define LIB_NCURSES_COLOR_GREEN           					"green"
#define LIB_NCURSES_COLOR_BLUE           	 				"blue"
#define LIB_NCURSES_COLOR_YELLOW          					"yellow"
#define LIB_NCURSES_COLOR_CYAN           	 				"cyan"
#define LIB_NCURSES_COLOR_MAGENTA          					"magenta"

/*---------------------------------------------------------------------------*/
#define LIB_NCURSES_COLOR_UNKNOWN           				8

/*---------------------------------------------------------------------------*/
#define LIB_NCURSES_KEYBOARD_COMBINATION_LEFT 				{27 ,91 ,68}
#define LIB_NCURSES_KEYBOARD_COMBINATION_RIGHT 				{27 ,91 ,67}
#define LIB_NCURSES_KEYBOARD_COMBINATION_ENTER 				10

/*-----------------------------------------------------------------------------
 * Lib ncurses type definitions
 *
 *---------------------------------------------------------------------------*/
typedef struct NcursesWorkers
{
    int colorID;

} NcursesWorkers;

/*---------------------------------------------------------------------------*/
typedef void IPCCallback(const String, const String);

/*-----------------------------------------------------------------------------
 * Lib ncurses window class
 *
 *---------------------------------------------------------------------------*/
class NcursesWindow
{
	public:
		std::list <NcursesWindow> 	fields;
		String						windowName;						
		
		NcursesWindow(const uint16_t xStart, const uint16_t yStart,
					  const uint16_t height, const uint16_t width, 
					  const bool border, const String name, WINDOW* parentWindow = NULL);
        ~NcursesWindow();

		int  			fieldCreate(const uint16_t xStart, const uint16_t yStart,
									const uint16_t height, const uint16_t width, 
									const bool border, const String field);

		int  			enableScrollMode(const bool mode);
		int  			enableScrollMode(const String field, const bool mode);

		int  			setWindowColor(String front, String background);
		int  			setWindowColor(const String field, String front, String background);

		int  			writeAppend(const String text);
		int  			writeAppend(const String field, const String text);

		int  			writeCentered(const String text);
		int  			writeCentered(const String field, const String text);

		bool 			fieldExists(const String name);
		NcursesWindow*	fieldGet(const String name);

		int				menuWindowInitialize(IPCCallback* callback);
		int				menuItemCreate(const uint16_t xStart, const uint16_t yStart,
									   const uint16_t height, const uint16_t width, 
									   const bool border, const String field,
									   const uint8_t id);
		int				menuItemSetAction(const uint8_t id, const String topic, const String message);
		int				menuItemChange(const bool next);
		int				menuItemIndicate(const bool indicate);
		int				menuItemActivate();

		NcursesWindow*	menuItemGet(const uint8_t id);

	private:

		WINDOW* 		mainWindow;
		int				mainWindowColor;

		IPCCallback* 	ipcCallback;

		bool			menuEnabled;
		uint8_t			currentItem;
		uint8_t			lastItem;
		uint8_t			menuID;
		String			menuItemName;
		String			menuActionTopic;
		String			menuActionMessage;
		int				keyboardInput[3];

		int  convertColor(const String colorString);

};

/*-----------------------------------------------------------------------------
 * Lib ncurses window class list
 *
 *---------------------------------------------------------------------------*/
typedef std::list<NcursesWindow> NcursesWindows;

/*-----------------------------------------------------------------------------
 * Lib ncurses exposed functions
 *
 *---------------------------------------------------------------------------*/
int  lib_ncurses_InitializeNcurses();
void lib_ncurses_DestroyNcurses();

/*---------------------------------------------------------------------------*/
NcursesWindow* lib_ncurses_GetWindow(NcursesWindows &ncursesWindows, const String windowID);
NcursesWindow* lib_ncurses_GetField(NcursesWindows &ncursesWindows, const String fieldID);
bool lib_ncurses_FieldExists(NcursesWindows &ncursesWindows, const String fieldID);

/*---------------------------------------------------------------------------*/
#endif /* LIB_NCURSES_H											             */
/*---------------------------------------------------------------------------*/