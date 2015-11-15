#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "console.h"

void setText(int posX, int posY, dBuff * dbuff, char * text){
    short atr = !(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED) | dbuff->buff1[posY][posX].atr;
    int i;
    for(i = 0; i < strlen(text); i++){
        dbuff->buff1[posY][i + posX].atr = atr;
        dbuff->buff1[posY][i + posX].text = *text + i;
    }
    moveCursor(posX, posY);
    setTextColor(atr);
    printf("%s", text);
 }

void drawField(int posX, int posY, int sizeX, int sizeY, dBuff *dbuff){
    int i, j;
    short sideAtr = FOREGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;

    for(i = posY + 1; i < posY + sizeY ; i++){
        for(j = posX + 1; j < posX + sizeX; j++){
            dbuff->buff2[i][j].atr = BACKGROUND_BLUE | BACKGROUND_GREEN;
            dbuff->buff2[i][j].text = ' ';
        }
    }

    for(i = posY; i <= posY + sizeY; i++){
        dbuff->buff2[i][posX].atr = sideAtr;
        dbuff->buff2[i][posX].text = '*';
        dbuff->buff2[i][posX + sizeX].atr = sideAtr;
        dbuff->buff2[i][posX + sizeX].text = '*';
    }

    for(i = posX; i <= posX + sizeX; i++){
        dbuff->buff2[posY][i].atr = sideAtr;
        dbuff->buff2[posY][i].text = '*';
        dbuff->buff2[posY + sizeY][i].atr = sideAtr;
        dbuff->buff2[posY + sizeY][i].text = '*';
    }

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLS; j++){
            if(dbuff->buff1[i][j].text != dbuff->buff2[i][j].text || dbuff->buff1[i][j].atr != dbuff->buff2[i][j].atr){
                moveCursor(j, i);
                setTextColor(dbuff->buff2[i][j].atr);
                printf("%c", dbuff->buff2[i][j].text);
            }
        }
    }
    switchBuffs(dbuff);
    clearBuff(dbuff->buff2);
}

int main()
{
    int size, i;
    dBuff dbuff;
    clearBuffs(&dbuff);
    char e[50] = "";
    do{
        drawField(COLS/4, ROWS/4, COLS/2, ROWS/2, &dbuff);
        setText(COLS/4 + 2, 3*ROWS/8, &dbuff, "Enter matrix size [1..9]:");
        setText(COLS/4 + 2, 3*ROWS/8 + 2, &dbuff, ">");
        scanf("%s", e);
        for(i = 0; i < strlen(e); i++)
            dbuff.buff1[3*ROWS/8 + 2][COLS/4 + 3 + i].text = 'b';
        size = atoi(e);
    }while(size > 9 || size < 1);

    while(1){
        drawField(0, 0, COLS - 1, 4 + size*2, &dbuff);
    }
    return 0;
}
