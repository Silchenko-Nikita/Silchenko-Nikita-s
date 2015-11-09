
#ifdef _WIN32  /* for windows os both 32bit or 64 bit */

#include <windows.h>

#define ATTR_DEFAULT 0
#define CLS_COMMAND "cls"

void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
}

void setTextColor(short color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color | FOREGROUND_INTENSITY);
}

void setBackgroundColor(short color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color | FOREGROUND_INTENSITY);
}

void clearScreen(void) {
    system(CLS_COMMAND);
}

void clearFormat(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, ATTR_DEFAULT);
}

void tSleep(unsigned int millis) {
    Sleep(millis);
}

#endif
