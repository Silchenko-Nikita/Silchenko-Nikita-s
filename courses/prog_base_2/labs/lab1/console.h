#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>

#include "terminal.h"

#define FOREGROUND_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define BACKGROUND_WHITE BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED

typedef enum {
    CONSOLE_OK,
    CONSOLE_INVALID_X,
    CONSOLE_INVALID_Y,
    CONSOLE_INVALID_WIDTH,
    CONSOLE_INVALID_HEIGHT
} Console_Error;

void Console_initialize(void);
Console_Error Console_setSize(short width, short height);
void Console_setTextColor(COLOR16 color);
void Console_setDefaultTextColor(void);
void Console_clear(void);
Console_Error Console_putCursor(short x, short y);
Console_Error Console_printField(short x, short y, short width, short height, COLOR16 color);
Console_Error Console_printTerminal(Terminal_t terminal, short x, short y, short width, short height, COLOR16 color, COLOR16 lineColor);
const char * Console_errorStr(Console_Error error);

#endif // CONSOLE_H_INCLUDED
