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
 * Lib ncurses include ncurses
 *
 *---------------------------------------------------------------------------*/
#include "lib_ncurses.h" 
NcursesWorkers ncursesWorkers;

/*-----------------------------------------------------------------------------
 * Lib ncurses global functions
 *
 *---------------------------------------------------------------------------*/
int lib_ncurses_InitializeNcurses()
{
	int returnCode = RETURN_CODE_OK;

	ncursesWorkers.colorID = 1;

	initscr();
	clear();

	setlocale(LC_ALL, "");

	cbreak();
	start_color();
	noecho();

	curs_set(0);

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void lib_ncurses_DestroyNcurses()
{
	nocbreak();
	endwin();
}

/*---------------------------------------------------------------------------*/
NcursesWindow* lib_ncurses_GetWindow(NcursesWindows &ncursesWindows, const String windowID)
{
	for (NcursesWindows::iterator ncursesWindowIt = ncursesWindows.begin(); 
		 ncursesWindowIt != ncursesWindows.end(); 
		 ncursesWindowIt++)
    {
        if (ncursesWindowIt->windowName == windowID)
        {
            return &(*ncursesWindowIt);
        }
    }

	return NULL;
}

/*---------------------------------------------------------------------------*/
NcursesWindow* lib_ncurses_GetField(NcursesWindows &ncursesWindows, const String fieldID)
{
	for (NcursesWindows::iterator ncursesWindowIt = ncursesWindows.begin(); 
		 ncursesWindowIt != ncursesWindows.end(); 
		 ncursesWindowIt++)
    {
		NcursesWindow* ncursesWindow = ncursesWindowIt->fieldGet(fieldID);

        if (ncursesWindow != NULL)
        {
            return ncursesWindow;
        }
    }

	return NULL;
}

/*---------------------------------------------------------------------------*/
bool lib_ncurses_FieldExists(NcursesWindows &ncursesWindows, const String fieldID)
{
	for (NcursesWindows::iterator ncursesWindowIt = ncursesWindows.begin(); 
		 ncursesWindowIt != ncursesWindows.end(); 
		 ncursesWindowIt++)
    {
		NcursesWindow* ncursesWindow = ncursesWindowIt->fieldGet(fieldID);

        if (ncursesWindow != NULL)
        {
            return true;
        }
    }

	return false;
}

/*-----------------------------------------------------------------------------
 * Lib ncurses class public functions
 *
 *---------------------------------------------------------------------------*/
NcursesWindow::NcursesWindow(const uint16_t xStart, const uint16_t yStart,
						  	 const uint16_t height, const uint16_t width, 
							 const bool border, const String name, WINDOW* parentWindow)
{
	init_pair(ncursesWorkers.colorID , COLOR_BLACK, COLOR_WHITE);
	mainWindowColor = 1;

	windowName = name;

	if (parentWindow != NULL)
	{
		mainWindow = derwin(parentWindow, height, width, yStart, xStart);
	}

	else
	{
		mainWindow = newwin(height, width, yStart, xStart);
	}

    wbkgd(mainWindow, COLOR_PAIR(ncursesWorkers.colorID++));

	if (border)
	{
    	box(mainWindow, 0, 0);
	}

    wrefresh(mainWindow);
}

/*---------------------------------------------------------------------------*/
NcursesWindow::~NcursesWindow()
{	
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::fieldCreate(const uint16_t xStart, const uint16_t yStart,
					  	       const uint16_t height, const uint16_t width, 
					  	       const bool border, const String field)
{
	if (fieldExists(field))
	{
		return RETURN_CODE_FAIL;
	}

	fields.emplace_back(xStart, yStart, height, width, border, field, mainWindow);

	return RETURN_CODE_OK;
}


/*---------------------------------------------------------------------------*/
int NcursesWindow::enableScrollMode(const bool mode)
{
	scrollok(mainWindow, mode);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::enableScrollMode(const String field, const bool mode)
{
	NcursesWindow* ncursesWindow = fieldGet(field);

	if (ncursesWindow == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	ncursesWindow->enableScrollMode(mode);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::setWindowColor(String front, String background)
{
	int frontColor 		= convertColor(front);
	int backgroundColor	= convertColor(background);

	if ((frontColor != LIB_NCURSES_COLOR_UNKNOWN) && (frontColor != LIB_NCURSES_COLOR_UNKNOWN))
	{
		init_pair(ncursesWorkers.colorID, frontColor, backgroundColor);
		wbkgd(mainWindow, COLOR_PAIR(ncursesWorkers.colorID++));

		wrefresh(mainWindow);

		return RETURN_CODE_OK;
	}

	return RETURN_CODE_FAIL;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::setWindowColor(const String field, String front, String background)
{
	NcursesWindow* ncursesWindow = fieldGet(field);

	if (ncursesWindow == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	ncursesWindow->setWindowColor(front, background);

	return RETURN_CODE_OK;
}


/*---------------------------------------------------------------------------*/
int NcursesWindow::writeAppend(const String text)
{
	wprintw(mainWindow, "%s\n", text.c_str());
	wrefresh(mainWindow);

	//printf("%s:%s\n", "field", text.c_str());

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::writeAppend(const String field, const String text)
{
	NcursesWindow* ncursesWindow = fieldGet(field);

	if (ncursesWindow == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	ncursesWindow->writeAppend(text);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::writeCentered(const String text)
{
	int yStart 		= 0,
		xStart 		= 0,
		textSize 	= 0,
		windowWidth	= 0;

	getmaxyx(mainWindow, yStart, windowWidth);
	textSize = text.length() / 2;

	yStart = yStart / 2;
	xStart = windowWidth / 2 - textSize;

	mvwprintw(mainWindow, yStart, xStart, text.c_str());
	wrefresh(mainWindow);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::writeCentered(const String field, const String text)
{
	NcursesWindow* ncursesWindow = fieldGet(field);

	if (ncursesWindow == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	ncursesWindow->writeCentered(text);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
bool NcursesWindow::fieldExists(const String name)
{
	for (NcursesWindows::iterator ncursesWindowIt = fields.begin(); 
		 ncursesWindowIt != fields.end(); 
		 ncursesWindowIt++)
    {
        if (ncursesWindowIt->windowName == name)
        {
            return true;
        }
    }

	return false;
}


/*---------------------------------------------------------------------------*/
NcursesWindow* NcursesWindow::fieldGet(const String name)
{
	for (NcursesWindows::iterator ncursesWindowIt = fields.begin(); 
		 ncursesWindowIt != fields.end(); 
		 ncursesWindowIt++)
    {
        if (ncursesWindowIt->windowName == name)
        {
            return &(*ncursesWindowIt);
        }
    }

	return NULL;
}

/*-----------------------------------------------------------------------------
 * Lib ncurses class menu functions
 *
 *---------------------------------------------------------------------------*/
int NcursesWindow::menuWindowInitialize(IPCCallback* callback)
{
	menuEnabled = true;
	ipcCallback = callback;

	currentItem 	= 1;
	lastItem		= 0;

	memset(keyboardInput, 0, sizeof(keyboardInput));

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::menuItemCreate(const uint16_t xStart, const uint16_t yStart,
								  const uint16_t height, const uint16_t width, 
								  const bool border, const String field,
								  const uint8_t id)
{
	int returnCode = fieldCreate(xStart, yStart, height, width, border, field);

	if (returnCode == RETURN_CODE_OK)
	{
		NcursesWindow* ncursesWindow = fieldGet(field);

		if (ncursesWindow == NULL)
		{
			return RETURN_CODE_ERR_UNKNOWN;
		}

		ncursesWindow->menuID 		= id;
		ncursesWindow->menuItemName = field;
		lastItem++;

		ncursesWindow->writeCentered(field);

		if (id == currentItem)
		{
			ncursesWindow->menuItemIndicate(true);
		}
	}
	
	return returnCode;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::menuItemSetAction(const uint8_t id, const String topic, const String message)
{
	NcursesWindow* ncursesWindow = menuItemGet(id);

	if (ncursesWindow == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	if (topic.empty())
	{
		return RETURN_CODE_FAIL;
	}

	ncursesWindow->menuActionTopic 		= topic;
	ncursesWindow->menuActionMessage 	= message;

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::menuItemChange(const bool next)
{
	NcursesWindow* ncursesWindowCurrent = menuItemGet(currentItem);

	if (ncursesWindowCurrent == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	ncursesWindowCurrent->menuItemIndicate(false);

	if (next)
	{
		if (currentItem == lastItem)
			currentItem = 1;

		else
			currentItem++;
	}

	else
	{
		if (currentItem == 1)
			currentItem = lastItem;

		else
			currentItem--;
	}

	NcursesWindow* ncursesWindowNext = menuItemGet(currentItem);

	if (ncursesWindowNext == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	ncursesWindowNext->menuItemIndicate(true);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::menuItemIndicate(const bool indicate)
{
	int yStart 		= 0,
		xStart 		= 0,
		windowWidth	= 0;

	getmaxyx(mainWindow, yStart, windowWidth);

	yStart = yStart / 2 + 1;
	xStart = windowWidth / 2 - 2;

	if (indicate)
	{
		mvwprintw(mainWindow, yStart, xStart, "----");
	}

	else
	{
		mvwprintw(mainWindow, yStart, xStart, "    ");
	}
	
	wrefresh(mainWindow);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
int NcursesWindow::menuItemActivate()
{
	NcursesWindow* ncursesWindow = menuItemGet(currentItem);

	if (ncursesWindow == NULL)
	{
		return RETURN_CODE_ERR_UNKNOWN;
	}

	if ((ncursesWindow->menuActionTopic.empty()) || (ipcCallback == NULL))
	{
		return RETURN_CODE_FAIL;
	}

	ipcCallback(ncursesWindow->menuActionTopic, ncursesWindow->menuActionMessage);

	return RETURN_CODE_OK;
}

/*---------------------------------------------------------------------------*/
NcursesWindow* NcursesWindow::menuItemGet(const uint8_t id)
{
	for (NcursesWindows::iterator ncursesWindowIt = fields.begin(); 
		 ncursesWindowIt != fields.end(); 
		 ncursesWindowIt++)
    {
        if (ncursesWindowIt->menuID == id)
        {
            return &(*ncursesWindowIt);
        }
    }

	return NULL;
}

/*-----------------------------------------------------------------------------
 * Lib ncurses class private functions
 *
 *---------------------------------------------------------------------------*/
int NcursesWindow::convertColor(const String colorString)
{
	if (colorString == LIB_NCURSES_COLOR_BLACK) return COLOR_BLACK;

	else if (colorString == LIB_NCURSES_COLOR_WHITE) return COLOR_WHITE;

	else if (colorString == LIB_NCURSES_COLOR_RED) return COLOR_RED;

	else if (colorString == LIB_NCURSES_COLOR_GREEN) return COLOR_GREEN;

	else if (colorString == LIB_NCURSES_COLOR_BLUE) return COLOR_BLUE;

	else if (colorString == LIB_NCURSES_COLOR_YELLOW) return COLOR_YELLOW;

	else if (colorString == LIB_NCURSES_COLOR_CYAN) return COLOR_CYAN;

	else if (colorString == LIB_NCURSES_COLOR_MAGENTA) return COLOR_MAGENTA;

	return LIB_NCURSES_COLOR_UNKNOWN;
}

/*---------------------------------------------------------------------------*/