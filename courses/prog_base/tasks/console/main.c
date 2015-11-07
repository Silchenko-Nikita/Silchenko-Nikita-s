#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

const int sleepTime = 15;
const int rows = 25;
const int cols = 80;

void update(COORD * pos, HANDLE hConsole){
    switch(((*pos).Y + (*pos).X < (2.0*cols)/3) ? (((*pos).Y + (*pos).X) % 3) : ( cols + rows - 2 - ((*pos).Y + (*pos).X)) % 3){
        case 0:
            SetConsoleTextAttribute(hConsole, (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE) | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
            break;
        case 1:
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
            break;
        case 2:
            SetConsoleTextAttribute(hConsole, !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE) | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
    }
    printf("*");
}

void goDown(COORD * pos, HANDLE hConsole){
    while((*pos).Y < rows){
        update(pos, hConsole);
        (*pos).Y++;
        SetConsoleCursorPosition(hConsole, *pos);
        Sleep(sleepTime);
    }
    (*pos).Y--;
}

void goUp(COORD * pos, HANDLE hConsole){
    while((*pos).Y >= 0){
        update(pos, hConsole);
        (*pos).Y--;
        SetConsoleCursorPosition(hConsole, *pos);
        Sleep(sleepTime);
    }
    (*pos).Y++;
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(hConsole, pos);
    while(pos.X < cols){
        goDown(&pos, hConsole);
        if(pos.X == cols - 1) break;
        pos.X++;
        SetConsoleCursorPosition(hConsole, pos);

        goUp(&pos, hConsole);
        pos.X++;
        SetConsoleCursorPosition(hConsole, pos);
    }
    return EXIT_SUCCESS;
}
