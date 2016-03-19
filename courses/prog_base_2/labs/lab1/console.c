#include <string.h>
#include <stdio.h>

#include "console.h"

static HANDLE hConsole;

static const short maxConsoleWidth = 150;
static const short maxConsoleHeight = 50;

static Console_Error checkArgs(short x, short y, short width, short height){
    if (x < 0 || x > maxConsoleWidth) return CONSOLE_INVALID_X;
    if (y < 0 || y > maxConsoleHeight) return CONSOLE_INVALID_Y;
    if (x + width < 0 || x + width > maxConsoleWidth) return CONSOLE_INVALID_WIDTH;
    if (y + height < 0 || y + height > maxConsoleHeight) return CONSOLE_INVALID_HEIGHT;
    return CONSOLE_OK;
}

void Console_initialize(void){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

Console_Error Console_setSize(short width, short height){
    Console_Error error = checkArgs(0, 0, width, height);
    if(error != CONSOLE_OK) return error;

    char cmd[32];
    sprintf(cmd, "mode con: lines=%d cols=%d", height, width);
    system(cmd);

    return CONSOLE_OK;
}

void Console_setTextColor(COLOR16 color){
    SetConsoleTextAttribute(hConsole, color);
}

void Console_setDefaultTextColor(void){
    SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);
}

void Console_clear(void){
    system("cls");
}

Console_Error Console_putCursor(short x, short y){
    Console_Error error = checkArgs(x, y, 0, 0);
    if(error != CONSOLE_OK) return error;

    COORD coord = {x, y};
    SetConsoleCursorPosition(hConsole, coord);

    return CONSOLE_OK;
}

Console_Error Console_printField(short x, short y, short width, short height, COLOR16 color){
    Console_Error error = checkArgs(x, y, width, height);
    if(error != CONSOLE_OK) return error;

    Console_setTextColor(color);

    for(short i = 0; i < height; i++){
        Console_putCursor(x, y + i);
        for(short j = 0; j < width; j++){
            putchar(' ');
        }
    }

    return CONSOLE_OK;
}

Console_Error Console_printTerminal(Terminal_t terminal, short x, short y, short width, short height, COLOR16 color, COLOR16 lineColor){
    Console_Error error = checkArgs(x, y, width, height);
    if(error != CONSOLE_OK) return error;

    Terminal_PrintSettings terminalPrintSett = Terminal_getPrintSettings(terminal);

    Console_printField(x, y, width, height, color);
    Console_putCursor(x + width/2 - 5, y + 1);
    printf("Terminal %d", Terminal_getId(terminal));

    Console_putCursor(x + 1, y + 4);
    printf("Prints:");
    if(terminalPrintSett == TERMINAL_PRINT_ALL){
        printf(" all");
    }else if(terminalPrintSett == TERMINAL_PRINT_NOBODY){
        printf(" nobody");
    }else{
        bool printComa = false;
        for(int i = 0; i < PERSON_CATEGORIES_NUMBER; i++){
            if(terminalPrintSett & ('\1' << i)){
                if(printComa){
                    printf(",");
                }else {
                    printComa = true;
                }
                printf(" %ss", Person_categoryStr(i));
            }
        }
    }

    Console_printField(x, y + 3, width, 1, lineColor);
    Console_printField(x, y + 5, width, 1, lineColor);

    Console_setTextColor(color);
    int passesNum = Wickets_getPassersNumber(Terminal_getWickets(terminal));
    int printed = 0;
    for(int i = 0; i < passesNum && printed < height - 5; i++){
        Person_t iPasser = Wickets_getPasser(Terminal_getWickets(terminal), i, NULL);
        Person_Category iPasserCategory = Person_getCategory(iPasser);

        if(terminalPrintSett & ('\1' << iPasserCategory)){
            Console_putCursor(x + 1, y + 6 + printed);
            printf("%s %s %s", Person_categoryStr(iPasserCategory), Person_getFirstName(iPasser), Person_getSecondName(iPasser));
            printed++;
        }
    }

    return CONSOLE_OK;
}

const char * Console_errorStr(Console_Error error){
    static const char * repr[] = {
        "OK",
        "Invalid x",
        "Invalid y",
        "Invalid width",
        "Invalid height"
    };

    return repr[error];
}
