#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define ROWS 24
#define COLS 80

#define BACK_BLACK !(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define FORE_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define FORE_BLACK !(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define FORE_YELLOW FOREGROUND_GREEN | FOREGROUND_RED
#define FIELD_COLOR BACKGROUND_GREEN | BACKGROUND_BLUE | FORE_BLACK
#define FIELD_BACKGROUND BACKGROUND_GREEN | BACKGROUND_BLUE

void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
}

void setTextColor(short color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clearScr(void){
    clearFormat();
    system("cls");
}

void clearFormat(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FORE_WHITE);
}

int setText(int posX, int posY, char *text, short color){
    char* r, k[100];
    int y = posY;
    strcpy(k, text);
    for( r = strtok(k, "\n"); r != NULL; r = strtok(NULL, "\n")){
        setTextLine(posX, y, r, color);
        y++;
    }
    return y - posY;
}

void getPartiedInts(char * text, int * first, int * second){
    char* r, k[100];
    int p = 0;
    strcpy(k, text);
    r = strtok(k, ",");
    if(r != NULL)
        *first = atoi(r);

    r = strtok(NULL, ",");
    if(r != NULL)
        *second = atoi(r);
}

void setTextLine(int posX, int posY, char *text, short color){
    setTextColor(color);
    moveCursor(posX, posY);
    printf("%s", text);
}

void getText(int posX, int posY, char *pointer, short backColor, short foreColor){
    setTextColor(backColor | (backColor == BACK_BLACK ? FORE_WHITE : FORE_BLACK));
    moveCursor(posX, posY);
    printf(">");
    setTextColor(backColor | foreColor);
    gets(pointer);
}

void drawField(int posX, int posY, int sizeX, int sizeY){
    int i, j;
    setTextColor(FORE_BLACK | BACKGROUND_BLUE);
    for(i = posY; i <= posY + sizeY; i++){
        moveCursor(posX, i);
        printf("*");
        moveCursor(posX + sizeX, i);
        printf("*");
    }

    for(i = posX; i <= posX + sizeX; i++){
        moveCursor(i, posY);
        printf("*");
        moveCursor(i, posY + sizeY );
        printf("*");
    }

    setTextColor(FIELD_COLOR);
    for(i = posY + 1; i < posY + sizeY ; i++){
        for(j = posX + 1; j < posX + sizeX; j++){
            moveCursor( j, i);
            printf(" ");
        }
    }
}

void fillRand(int length, int height, int matr[height][length], int low, int high){
    int i, j;
    for(i = 0; i < height; i++){
        for(j = 0; j < length; j++){
            matr[i][j] = low + rand() % (high - low + 1);
        }
    }
}

void printMatr(int length, int height, int matr[height][length], int isTurned, int toSwitch, short color){
    int i, j, initX, x, y;
    setTextColor(color);

    y = 2;
    if(!isTurned){
        if(toSwitch){
            clearScr();
            drawField(0, 0, COLS - 1, 2 + height*2);
        }
        initX = (COLS - length*6)/2;
        for(i = 0; i < height; i++){
            x = initX;
            for(j = 0; j < length; j++){
                moveCursor(x, y);
                printf("%5d", matr[i][j]);
                x += 6;
            }
            y +=2;
        }
    }else{
        if(toSwitch){
            clearScr();
            drawField(0, 0, COLS - 1, 2 + length*2);
        }
        initX = (COLS - height*6)/2;
        for(i = 0; i < length; i++){
            x = initX;
            for(j = 0; j < height; j++){
                moveCursor(x, y);
                printf("%5d", matr[j][i]);
                x += 6;
            }
            y +=2;
        }
    }
}

void swapCols(int length, int height, int matr[height][length], int isTurned, int* first, int *second){
    int i, j, k, sumMin = 0, sumMax = 0, sum, minPos = 0, maxPos = 0;
    if(!isTurned){
        for(j = 0; j < height; j++){
            sumMax += matr[0][j];
        }
        sumMin = sumMax;
        for(i = 1; i < length; i++){
            sum = 0;
            for(j = 0; j < height; j++){
                sum += matr[j][i];
            }
            if(sum > sumMax){
                sumMax = sum;
                maxPos = i;
            }
            if(sum < sumMin){
                sumMin = sum;
                minPos = i;
            }
        }
        for(j = 0; j < height; j++){
            k = matr[j][minPos];
            matr[j][minPos] = matr[j][maxPos];
            matr[j][maxPos] = k;
        }
    }else{
        for(j = 0; j < length; j++){
            sumMax += matr[0][j];
        }
        sumMin = sumMax;
        for(i = 1; i < height; i++){
            sum = 0;
            for(j = 0; j < length; j++){
                sum += matr[i][j];
            }
            if(sum > sumMax){
                sumMax = sum;
                maxPos = i;
            }
            if(sum < sumMin){
                sumMin = sum;
                minPos = i;
            }
        }
        for(j = 0; j < length; j++){
            k = matr[minPos][j];
            matr[minPos][j] = matr[maxPos][j];
            matr[maxPos][j] = k;
        }
    }

    *first = minPos;
    *second = maxPos;
}

void clearMatr(int length, int height, int matr[height][length]){
    int i, j;
    for(i = 0; i < height; i++){
        for(j = 0; j < length; j++){
            matr[i][j] = 0;
        }
    }
}

int sumInRow(int length, int height, int matr[height][length], int isTurned, int row){
    int i, j, sum = 0;
    if(!isTurned){
        for(j = 0; j < length; j++){
            sum += matr[row][j];
        }
    }else{
        for(j = 0; j < height; j++){
            sum += matr[j][row];
        }
    }
    return sum;
}

double countMiddle(int length, int height, int matr[height][length]){
    int i, j, k = height*length, sum = 0;
    for(i = 0; i < height; i++){
        for(j = 0; j < length; j++){
            sum += matr[i][j];
        }
    }
    return (sum*1.0/k);
}

void helpComm(int y){
    short yellow = FORE_YELLOW;
    short x = 3;
    setTextLine(x, y, "clear all", yellow);
    setTextLine(COLS/3, y, "to set all elements to 0", FORE_WHITE);
    y++;

    setTextLine(x, y, "random %min%, %max%", yellow);
    setTextLine(COLS/3, y, "to fill elements with random value from min to max", FORE_WHITE);
    y++;

    setTextLine(x, y, "set %x%, %y% : %val%", yellow);
    setTextLine(COLS/3, y, "to set (x, y) element to val", FORE_WHITE);
    y++;

    setTextLine(x, y, "print upright", yellow);
    setTextLine(COLS/3, y, "to print matrix upright", FORE_WHITE);
    y++;

    setTextLine(x, y, "middle value", yellow);
    setTextLine(COLS/3, y, "to show middle arithmetics value of all elements", FORE_WHITE);
    y++;

    setTextLine(x, y, "sum in row %row%", yellow);
    setTextLine(COLS/3, y, "to show sum value of elements in row %row%", FORE_WHITE);
    y++;

    setTextLine(x, y, "swap 1", yellow);
    setTextLine(COLS/3, y, "to swap first min with last max", FORE_WHITE);
    y++;

    setTextLine(x, y, "swap 2", yellow);
    setTextLine(COLS/3, y, "to swap last min with first max", FORE_WHITE);
    y++;

    setTextLine(x, y, "swap cols", yellow);
    setTextLine(COLS/3, y, "to swap column with max sum with column with min sum", FORE_WHITE);
    y++;

    setTextLine(x, y, "clean workspace", yellow);
    setTextLine(COLS/3, y, "to clean space under the field", FORE_WHITE);
    y++;

    setTextLine(x, y, "exit", yellow);
    setTextLine(COLS/3, y, "to exit the program", FORE_WHITE);
}

void clean(y){
    int i, j;
    setTextColor(0);
    for(i = 0; i < 13; i++){
        for(j = 2; j < COLS; j++){
            moveCursor(j, i + y + 3);
            putchar(' ');
        }
    }
}

void invalidInput(int y){
    setTextLine(5, y + 4, "Invalid input!                       ", FOREGROUND_RED);
    Sleep(1500);
    setTextLine(5, y + 4, "                ", 0);
}

void printEl(int length, int height, int matr[height][length], int isTurned, int cX, int cY, short color){
    if(!isTurned){
        moveCursor((COLS - length*6)/2 + 6*cX, (cY + 1)*2);
        setTextColor(color);
        printf("%5d", matr[cY][cX]);
    }else{
        moveCursor((COLS - height*6)/2 + 6*cX, (cY + 1)*2);
        setTextColor(color);
        printf("%5d", matr[cX][cY]);
    }
}

void swapMinMax(int length, int height, int matr[height][length], int isTurned, int firstForm, COORD coord[2]){
    int i, j, min = matr[0][0], max = matr[0][0], k;
    coord[0].X = 0;
    coord[0].Y = 0;
    coord[1].X = 0;
    coord[1].Y = 0;
    if(!isTurned){
        if(firstForm){
            for(i = 0; i < height; i++){
                for(j = 0; j < length; j++){
                    if(min > matr[i][j]){
                        min = matr[i][j];
                        coord[0].X = j;
                        coord[0].Y = i;
                    }
                    if(max <= matr[i][j]){
                        max = matr[i][j];
                        coord[1].X = j;
                        coord[1].Y = i;
                    }
                }
            }
        }else{
            for(i = 0; i < height; i++){
                for(j = 0; j < length; j++){
                    if(min >= matr[i][j]){
                        min = matr[i][j];
                        coord[0].X = j;
                        coord[0].Y = i;
                    }
                    if(max < matr[i][j]){
                        max = matr[i][j];
                        coord[1].X = j;
                        coord[1].Y = i;
                    }
                }
            }
        }
    }else{
        if(firstForm){
            for(i = 0; i < length; i++){
                for(j = 0; j < height; j++){
                    if(min > matr[j][i]){
                        min = matr[j][i];
                        coord[0].X = i;
                        coord[0].Y = j;
                    }
                    if(max <= matr[j][i]){
                        max = matr[j][i];
                        coord[1].X = i;
                        coord[1].Y = j;
                    }
                }
            }
        }else{
            for(i = 0; i < length; i++){
                for(j = 0; j < height; j++){
                    if(min >= matr[j][i]){
                        min = matr[j][i];
                        coord[0].X = i;
                        coord[0].Y = j;
                    }
                    if(max < matr[j][i]){
                        max = matr[j][i];
                        coord[1].X = i;
                        coord[1].Y = j;
                    }
                }
            }
        }
    }
    k = matr[coord[0].Y][coord[0].X];
    matr[coord[0].Y][coord[0].X] = matr[coord[1].Y][coord[1].X];
    matr[coord[1].Y][coord[1].X] = k;
}

void pickOut(int length, int height, int matr[height][length], int isTurned, COORD coord, short color){
    int x, y;
    setTextColor(color);
    if(!isTurned){
         x = (COLS - length*6)/2 + 6*coord.X;
         y = (coord.Y + 1)*2;
         moveCursor(x, y);
         printf("%5d", matr[coord.Y][coord.X]);
    }else{
         x = (COLS - height*6)/2 + 6*coord.Y;
         y = (coord.X + 1)*2;
         moveCursor(x, y);
         printf("%5d", matr[coord.Y][coord.X]);
    }
}

void pickOutRow(int length, int height, int matr[height][length], int isTurned, int row, short color){
    int i, x, y;
    setTextColor(color);
    if(!isTurned){
         x = (COLS - length*6)/2;
         y = (row + 1)*2;
         for(i = 0; i < length; i++){
            moveCursor(x + 6*i, y);
            printf("%5d", matr[row][i]);
         }
    }else{
         x = (COLS - height*6)/2;
         y = (row + 1)*2;
         for(i = 0; i < height; i++){
            moveCursor(x + 6*i, y);
            printf("%5d", matr[i][row]);
         }
    }
}

void pickOutCol(int length, int height, int matr[height][length], int isTurned, int col, short color){
    int i, x, y;
    setTextColor(color);
    if(!isTurned){
         x = (COLS - length*6)/2 + (col + 1)*6;
         y = 2;
         for(i = 0; i < height; i++){
            moveCursor(x, y*(i + 1));
            printf("%5d", matr[i][col + 1]);
         }
    }else{
         x = (COLS - height*6)/2 + (col + 1)*6;
         y = 2;
         for(i = 0; i < length; i++){
            moveCursor(x, y*(i + 1));
            printf("%5d", matr[col + 1][i]);
         }
    }
}

int main(void)
{
    int length = 0, height = 0, i, k, lowVal = -1000, highVal = -1000;
    char entred[100] = "";

    do{
        drawField(COLS/4, ROWS/4, COLS/2, ROWS/2);
        k = setText(COLS/4 + 2, 3*ROWS/8, "Enter matrix length [1..9]\n& height [1..9]\nparting values with coma:", FIELD_COLOR);
        getText(COLS/4 + 2, 3*ROWS/8 + k + 1, entred, FIELD_BACKGROUND, FORE_BLACK);
        getPartiedInts(entred, &length, &height);
        if((length > 9 || length < 1) || (height > 9 || height< 1)){
            setTextLine(COLS/4 + 2, 3*ROWS/8 + k + 4, "Invalid input!", FIELD_BACKGROUND | FOREGROUND_RED);
            Sleep(1500);
        }
    }while((length > 9 || length < 1) || (height > 9 || height< 1));

    do{
        drawField(COLS/4, ROWS/4, COLS/2, ROWS/2);
        k = setText(COLS/4 + 2, 3*ROWS/8, "Enter matrix random values diapason\nparting left & right bounds with coma:", FIELD_COLOR);
        getText(COLS/4 + 2, 3*ROWS/8 + k + 1, entred, FIELD_BACKGROUND, FORE_BLACK);
        getPartiedInts(entred, &lowVal, &highVal);
        if((lowVal > highVal) || (highVal > 999) || (lowVal < -999)){
            setTextLine(COLS/4 + 2, 3*ROWS/8 + k + 4, "Invalid input!", FIELD_BACKGROUND | FOREGROUND_RED);
            Sleep(1500);
        }
    }while((lowVal > highVal) || (highVal > 999) || (lowVal < -999));


    int matr[height][length], mcpy[length][height], isTurned = 0, toSwitch = 0, firstToSwap, secondToSwap;
    int cX, cY, val, row;
    fillRand(length, height, matr, lowVal, highVal);

    int y = 2 + height*2;

    short color = FIELD_COLOR, toClean = 0, isSwaped = 0, help = 0, toPickOutCols = 0, toYellow = 0, toPickOutRow = 0;
    char * g, cpy[100];
    COORD swaped[2];
    clearScr();
    drawField(0, 0, COLS - 1, y);

    while(1){
        y = isTurned ? (2 + length*2) : (2 + height*2);
        if(toClean){
            toClean = 0;
            clean(y);
        }
        printMatr(length, height, matr, isTurned, toSwitch, color);
        color = FIELD_COLOR;

        if(isSwaped){
            pickOut(length, height, matr, isTurned, swaped[0], BACKGROUND_GREEN);
            pickOut(length, height, matr, isTurned, swaped[1], BACKGROUND_RED);
            isSwaped = 0;
        } else if(toYellow){
            printEl(length, height, matr, isTurned, cX - 1, cY - 1, FORE_YELLOW | FIELD_BACKGROUND | FOREGROUND_INTENSITY);
            toYellow = 0;
        } else if(toPickOutRow){
            pickOutRow(length, height, matr, isTurned, row - 1, BACKGROUND_GREEN);
            toPickOutRow = 0;
        } else if(toPickOutCols){
            pickOutCol(length, height, matr, isTurned, firstToSwap - 1, BACKGROUND_GREEN);
            pickOutCol(length, height, matr, isTurned, secondToSwap - 1, BACKGROUND_RED);
            toPickOutCols = 0;
        }

        toSwitch = 0;
        setTextLine(1, y + 2, "Enter your command:",  FORE_WHITE);
        do{
            getText(1, y + 3, entred, BACK_BLACK, FOREGROUND_GREEN);
        }while(entred[0] == '\0');

        strcpy(cpy, entred);
        g = strtok(cpy, " ");
        if(!g)
            g = "\n";
        for(i = 2; i < COLS; i++){
            moveCursor(i, y + 3);
            putchar(' ');
        }
        if(!strcmp(entred, "help")){
            helpComm(y + 5);
            help = 1;
        }else if(!strcmp(entred, "clear all")){
            clearMatr(length, height, matr);
            color = FIELD_BACKGROUND | FORE_YELLOW | FOREGROUND_INTENSITY;
        }else if(!strcmp(g, "random")){
            lowVal = -1000;
            highVal = -1000;
            char cpy1[100];
            for(i = 0; cpy[i + 7] != '\0'; i++){
                cpy1[i] = cpy[i + 7];
            }
            cpy1[i] = '\0';
            getPartiedInts(cpy1, &lowVal, &highVal);

            if((lowVal > highVal) || (highVal > 999) || (lowVal < -999)){
                invalidInput(y);
            }else{
                fillRand(length, height, matr, lowVal, highVal);
                color = FIELD_BACKGROUND | FORE_YELLOW | FOREGROUND_INTENSITY;
            }
        }else if(!strcmp(g, "set")){
            cX = -1;
            cY = -1;
            val = -1000;
            char cpy1[100], *l;
            for(i = 0; cpy[i + 4] != '\0'; i++){
                cpy1[i] = cpy[i + 4];
            }
            cpy1[i] = '\0';
            getPartiedInts(cpy1, &cX, &cY);
            l = strtok(cpy1, ":");
            l = strtok(NULL, ":");
            if(l != NULL){
                val = atoi(l);
            }

            if(!isTurned){
                if(cX > length || cX <= 0 || cY > height || cY <= 0 || val > 999 || val < -999){
                    invalidInput(y);
                }else{
                    matr[cY -  1][cX - 1] = val;
                    toYellow = 1;
                }
            }else{
                if(cX > height || cX <= 0 || cY > length || cY <= 0 || val > 999 || val < -999){
                    invalidInput(y);
                }else{
                    matr[cX - 1][cY - 1] = val;
                    toYellow = 1;
                }
            }
        }else if(!strcmp(entred, "print upright")){
            isTurned = !isTurned;
            toSwitch = 1;
        }else if(!strcmp(entred, "middle value")){
            double mid = countMiddle(length, height, matr);
            setTextColor(FORE_WHITE);
            moveCursor(5, y + 4);
            printf("middle value: ");
            setTextColor(FORE_YELLOW);
            moveCursor(22, y + 4);
            printf("%f                         ", mid);
            help = 1;
        }else if(!strcmp(g, "sum")){
            int sum;
            row = -1;
            g = strtok(NULL, " ");
            if(g != NULL && !strcmp(g, "in")){
                g = strtok(NULL, " ");
                if(g != NULL && !strcmp(g, "row")){
                    g = strtok(NULL, " ");
                    if(g != NULL)
                        row = atoi(g);
                }
            }
            if(row < 0 || row > (isTurned ? length : height))
                invalidInput(y);
            else{
                sum = sumInRow(length, height, matr, isTurned, row -1);
                toPickOutRow = 1;
                setTextColor(FORE_WHITE);
                moveCursor(5, y + 4);
                printf("sum in %dth row: ", row);
                setTextColor(FORE_YELLOW);
                moveCursor(22, y + 4);
                printf("%d                         ", sum);
                help = 1;
            }
        }else if(!strcmp(entred, "swap 1")){
            swapMinMax(length, height, matr, isTurned, 1, swaped);
            isSwaped = 1;
        }else if(!strcmp(entred, "swap 2")){
            swapMinMax(length, height, matr, isTurned, 0, swaped);
            isSwaped = 1;
        }else if(!strcmp(entred, "swap cols")){
            swapCols(length, height, matr, isTurned, &firstToSwap, &secondToSwap);
            toPickOutCols = 1;
        }else if(!strcmp(entred, "clean workspace") && help){
            toClean = 1;
            help = 0;
        }else if(!strcmp(entred, "fuck you")){
            clearScr();
            setTextLine(0, 0, "GO FUCK YOURSELF!", FOREGROUND_RED | FOREGROUND_INTENSITY);
            setTextColor(FORE_WHITE);
            return EXIT_FAILURE;
        }else if(!strcmp(entred, "exit")){
            clearScr();
            setTextColor(FORE_WHITE);
            return EXIT_SUCCESS;
        }else{
            invalidInput(y);
        }
    }
    return EXIT_SUCCESS;
}
