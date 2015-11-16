#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define ROWS 24
#define COLS 79

float countF1(float x){
    return 1.5 * cos(0.2 * x);
}

float countF2(float x){
    return -2 / (4 * x - 1) - 1;
}

int intround(float a){
    int res;
    if(a > 0){
        res = a - (int) a >= 0.5 ? (int) a + 1 : (int) a;
    }else{
        res = a + (int) a <= -0.5 ? (int) a - 1 : (int) a;
    }
    return res;
}

void drawF1(float k, HANDLE hConsole, int biasX, int biasY){
    float i, x;
    COORD point;
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN);
    for(i = 0; i <= COLS; i += 0.2){
        x = (i - COLS/2 + biasX)/k;
        point.X = i;
        point.Y = intround(-k*countF1((i - COLS/2 + biasX)/k) + ROWS/2 + biasY);
        if(point.Y >= 0 && point.Y <=ROWS){
            SetConsoleCursorPosition(hConsole, point);
            printf(" ");
        }
    }
}

void drawF2(float k, HANDLE hConsole, int biasX, int biasY){
    float i, x;
    COORD point;
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
    for(i = 0; i <= COLS; i = abs(x) < 0.5 ? i + 0.01 : i + 0.2){
        x = (i - COLS/2 + biasX)/k;
        point.X = i;
        point.Y = intround(-k*countF2(x) + ROWS/2 + biasY);

        if(point.Y >= 0 && point.Y <=ROWS){
            SetConsoleCursorPosition(hConsole, point);
            printf(" ");
        }
    }
}

void drawDec(float k, HANDLE hConsole, int biasX, int biasY){
    int i;
    COORD point;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    point.Y = ROWS/2 + biasY;
    for(i = 0; i <= COLS/2; i++){
        point.X = i;
        SetConsoleCursorPosition(hConsole, point);
        if((i + biasX) % (int) (k + 0.5) == 0)
            printf(".");
        else
            printf("-");

        point.X = i + COLS/2;
        SetConsoleCursorPosition(hConsole, point);
        if((i + biasX) % (int) (k + 0.5) == 0)
            printf(".");
        else
            printf("-");
    }

    point.X = COLS/2 - biasX;
    for(i = 0; i <= ROWS/2; i++){
        point.Y = i;

        SetConsoleCursorPosition(hConsole, point);
        if((i - biasY) % (int) (k + 0.5) == 0)
            printf(".");
        else
            printf("|");

        point.Y = i + ROWS/2;
        SetConsoleCursorPosition(hConsole, point);
        if((i - biasY) % (int) (k + 0.5) == 0)
            printf(".");
        else
            printf("|");
    }
}

void initText(HANDLE hConsole){
    COORD point;
    point.X = 1;
    point.Y = 1;
    SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_BLUE);
    SetConsoleCursorPosition(hConsole, point);
    printf(" ");
    point.X = 2;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    SetConsoleCursorPosition(hConsole, point);
    printf(" 1.5 * cos(0.2 * x)");

    point.X = 1;
    point.Y = 3;
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
    SetConsoleCursorPosition(hConsole, point);
    printf(" ");
    point.X = 2;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    SetConsoleCursorPosition(hConsole, point);
    printf(" -2 / (4 * x - 1) - 1");

    point.X = 1;
    point.Y = ROWS - 3;
    SetConsoleCursorPosition(hConsole, point);
    printf("press \"+\" to increase scale");

    point.X = 1;
    point.Y = ROWS - 1;
    SetConsoleCursorPosition(hConsole, point);
    printf("press \"-\" to decrease scale");

    point.X = COLS/2 + 5;
    point.Y = ROWS - 1;
    SetConsoleCursorPosition(hConsole, point);
    printf("press \"w, a, s, d\" to move graph");
}

void update(float k, HANDLE hConsole, int biasX, int biasY){
    initText(hConsole);
    drawDec(k, hConsole, biasX, biasY);
    drawF1(k, hConsole, biasX, biasY);
    drawF2(k, hConsole, biasX, biasY);
}

void clearConsole(HANDLE hConsole){
    SetConsoleTextAttribute(hConsole, 0);
    system("cls");
}

int main(void)
{
    float unitLength = 1, k = 3;
    int unitCount = COLS/unitLength, j, x, biasX = 0, biasY = 0, stepL = 2;
    char ch;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    do{
        update(k, hConsole, biasX, biasY);
        ch = getch();
        switch(ch){
            case '+':
                k = k < 7 ? 1.1*k : k;
            break;
            case '-':
                k = k > 0.7 ? 0.9*k : k;
            break;
            case 'd':
                biasX = biasX < (COLS/2 - 5) ? biasX + stepL : biasX;
            break;
            case 's':
                biasY = biasY > (-ROWS/2 + 5) ? biasY - stepL : biasY;
            break;
            case 'a':
                biasX = biasX > (-COLS/2 + 5) ? biasX - stepL : biasX;
            break;
            case 'w':
                biasY = biasY < (ROWS/2 - 5) ? biasY + stepL : biasY;
        }
        clearConsole(hConsole);
    }while(1);
    return 0;
}
