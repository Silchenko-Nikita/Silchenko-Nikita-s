#include <windows.h>
#include "console.h"

#define ATTR_DEFAULT 0
#define CLS_COMMAND "cls"

void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
}

void setTextColor(short color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clearBuff(Cell buff[ROWS][COLS]){
    int i, j;
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            buff[i][j].text = ' ';
            buff[i][j].atr = ATTR_DEFAULT;
        }
    }
}

void clearBuffs(dBuff *dbuff) {
    int i, j;
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            dbuff->buff1[i][j].text = ' ';
            dbuff->buff1[i][j].atr = ATTR_DEFAULT;
            dbuff->buff2[i][j].text = ' ';
            dbuff->buff2[i][j].atr = ATTR_DEFAULT;
        }
    }
}

void switchBuffs(dBuff* dbuff){
    int i, j;
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            dbuff->buff1[i][j].text = dbuff->buff2[i][j].text;
            dbuff->buff1[i][j].atr = dbuff->buff2[i][j].atr;
        }
    }
}

void clearFormat(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, ATTR_DEFAULT | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
