#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "wickets.h"

#define TERMINAL_PRINT_NOBODY '\0'
#define TERMINAL_PRINT_STUDENTS '\1'
#define TERMINAL_PRINT_TEACHERS '\2'
#define TERMINAL_PRINT_PERSONNEL '\4'
#define TERMINAL_PRINT_ALL '\7'

typedef struct Terminal * Terminal_t;
typedef unsigned char Terminal_PrintSettings;

typedef enum {
    TERMINAL_OK,
    TERMINAL_SETTINGS_ERROR
} Terminal_Error;

Terminal_t Terminal_new(unsigned int id, Terminal_PrintSettings termPrintSett, Wickets_t wickets, Terminal_Error * error);
void Terminal_delete(Terminal_t self);
void Terminal_deepDelete(Terminal_t self);
unsigned int Terminal_getId(Terminal_t self);
Terminal_Error Terminal_setPrintSettings(Terminal_t self, Terminal_PrintSettings termPrintSett);
Terminal_PrintSettings Terminal_getPrintSettings(Terminal_t self);
Wickets_t Terminal_setWickets(Terminal_t self, Wickets_t wickets);
Wickets_t Terminal_getWickets(Terminal_t self);
const char * Terminal_errorStr(Terminal_Error error);

#endif // TERMINAL_H_INCLUDED
