#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define MAX_STATES 4
#define FIGURES_NUMBER 7
#define BLOCKS_NUMBER 4
#define LEFT_ARR 'a'
#define RIGHT_ARR 'd'
#define UP_ARR 'w'
#define DOWN_ARR 's'
#define IS_ENGAGED true
#define BACK_WHITE BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED
#define FORE_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED

typedef short Color;
typedef struct Pos{int X, Y} Pos;
typedef struct Figure{int statesNum; Pos pos[MAX_STATES][BLOCKS_NUMBER]; Color color} Figure;
typedef struct ActingFigure{Figure figure; int state; Pos bias; bool isAccelerating} ActingFigure;
typedef struct Stat{int level, linesDestroyed, score; char nickname[]} Stat;
typedef struct ToGiveThread{bool *field; ActingFigure * curFigure; HANDLE hMutex} ToGiveThread;

const short maxLevel = 9;
const short toDestroyLinesBeforeRaise = 10;
const char * const leadersTableFile = "LeadersTable.txt";
const short tetrFieldWidth = 10;
const short tetrFieldHeight = 20;
const short fieldX = 22;
const short fieldY = 1;


void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
}

void setTextColor(Color color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void updateField(Color fieldCol[tetrFieldHeight][tetrFieldWidth]){
    int i, j;
    int x = fieldX + 1, y = fieldY + 1;
    for(i = 0; i < tetrFieldHeight; i++){
        for(j = 0; j < tetrFieldWidth; j++){
            setTextColor(fieldCol[i][j]);
            moveCursor(x + j, y + i);
            putchar(' ');
        }
    }
}

void updateFilledLine(bool field[tetrFieldHeight][tetrFieldWidth], Color fieldCol[tetrFieldHeight][tetrFieldWidth], int lineN){
    int i, j;
    for(i = lineN ; i > 0; i--){
        for(j = 0; j < tetrFieldWidth; j++){
            field[i][j] = field[i - 1][j];
            fieldCol[i][j] = fieldCol[i - 1][j];
        }
    }
    for(j = 0; j < tetrFieldWidth; j++){
        field[0][j] = !IS_ENGAGED;
        fieldCol[0][j] = BACK_WHITE;
    }
}

int lineIsFiled(bool field[tetrFieldHeight][tetrFieldWidth]){
    int i, j;
    bool check;
    for(i = 0; i < tetrFieldHeight; i++){
        check = true;
        for(j = 0; j < tetrFieldWidth; j++){
            if(field[i][j] == !IS_ENGAGED){
                check = false;
                break;
            }
        }
        if(check) return i;
    }
    return -1;
}

void whileLineIsFilled_Update(bool field[tetrFieldHeight][tetrFieldWidth], Color fieldCol[tetrFieldHeight][tetrFieldWidth], Stat * stat){
    int line;
    bool check = false;
        while((line = lineIsFiled(field)) != -1){
            updateFilledLine(field, fieldCol, line);
            check = true;

            stat->linesDestroyed++;
            stat->score += stat->level;
            if((stat->linesDestroyed - 1)/toDestroyLinesBeforeRaise < stat->linesDestroyed/toDestroyLinesBeforeRaise)
                if(stat->level < maxLevel) stat->level++;
        }
        if(check) updateField(fieldCol);
}

void initField(){
    drawField(fieldX, fieldY, tetrFieldWidth  + 23, tetrFieldHeight + 2, BACK_WHITE | BACKGROUND_INTENSITY);
    drawField(fieldX + 1, fieldY + 1, tetrFieldWidth, tetrFieldHeight, BACK_WHITE);
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
    moveCursor(fieldX + tetrFieldWidth + 2, fieldY + tetrFieldHeight);
    printf("Use \"w, a, s, d\"");
}

void clearFieldBuf(bool field[tetrFieldHeight][tetrFieldWidth], Color fieldCol[tetrFieldHeight][tetrFieldWidth]){
    int i, j;
    for(i = 0; i < tetrFieldHeight; i++){
        for(j = 0; j < tetrFieldWidth; j++){
            field[i][j] = !IS_ENGAGED;
            fieldCol[i][j] = BACK_WHITE;
        }
    }
}

void drawField(int x, int y, int width, int height, Color color){
    setTextColor(color);
    int i, j;
    for(i = x; i < x + width; i++){
        for(j = y; j < y + height; j++){
            moveCursor(i, j);
            putchar(' ');
        }
    }
}

int compareStat(const void * a, const void * b){
    char a1[50], b1[50];
    int a2, b2;
    strcpy(a1, a);
    strcpy(b1, b);

    strtok(a1, " ");
    a2 = atoi(strtok(NULL, " "));

    strtok(b1, " ");
    b2 = atoi(strtok(NULL, " "));

    return b2 - a2;
}

void initStat(Stat * stat){
    setTextColor(0);
    system("cls");
    do{
        drawField(fieldX, 0, 40, 4, BACK_WHITE | BACKGROUND_INTENSITY);
        setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
        moveCursor(fieldX + 2, 0);
        printf("Please, enter level(from 1 to 9):");
        moveCursor(fieldX + 2, 1);
        scanf("%d", &stat->level);
    }while(stat->level > maxLevel || stat->level <= 0);

    drawField(fieldX, 0, 40, 4, BACK_WHITE | BACKGROUND_INTENSITY);
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
    moveCursor(fieldX + 2, 0);
    printf("Please, enter your nickname:");
    moveCursor(fieldX + 2, 1);
    scanf("%s", stat->nickname);
    setTextColor(0);
    system("cls");

    stat->score = 0;
    stat->linesDestroyed = 0;
}

void initLeaderboardFile(){
    FILE * f = fopen(leadersTableFile, "w");
    if(NULL == f) exit(1);
    int i;
    for(i = 0; i < 10; i++){
        fputs("* 0\n", f);
    }
    fclose(f);
}

void showScore(){
    char d[10][50];
    int i;
    FILE * f = fopen(leadersTableFile, "r");
    if(NULL == f){
        initLeaderboardFile();
        fclose(f);
        f = fopen(leadersTableFile, "r");
        if(NULL == f) exit(1);
    }
    for(i = 0; i < 10; i++){
        fgets(d[i], 50, f);
    }
    drawField(fieldX, 0, 40, 24, BACK_WHITE | BACKGROUND_INTENSITY);
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
    moveCursor(fieldX + 2, 0);
    printf("LEADERBOARD:");
    for(i = 0; i < 10; i++){
        moveCursor(fieldX + 2, 2*i + 2);
        printf(d[i]);
    }
    moveCursor(fieldX + 2, 23);
    printf("Press any key to continue...");
    getch();
    fclose(f);
}

void updateScore(Stat * stat){
    char d[11][50];
    FILE * f = fopen(leadersTableFile, "r");
    if(NULL == f) exit(1);
    int i;
    for(i = 0; i < 10; i++){
        fgets(d[i], 50, f);
    }
    f = freopen(leadersTableFile, "w", f);
    sprintf(d[10], "%s %d\n", stat->nickname, stat->score);
    qsort(d, 11, sizeof(char)*50, compareStat);
    for(i = 0; i < 10; i++){
        fputs(d[i], f);
    }
    fclose(f);
}

bool tryGoDown(bool field[tetrFieldHeight][tetrFieldWidth], Color fieldCol[tetrFieldHeight][tetrFieldWidth], ActingFigure * curFigure, Stat * stat){
    int i, j;
    int x = fieldX + 1, y = fieldY + 1;
    bool check;
    for(i = 0; i < BLOCKS_NUMBER; i++){
        if(field[curFigure->figure.pos[curFigure->state][i].Y + 1 + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][i].X + curFigure->bias.X] == IS_ENGAGED){
            for(j =0; j < BLOCKS_NUMBER; j++){
                field[curFigure->figure.pos[curFigure->state][j].Y  + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][j].X + curFigure->bias.X] = IS_ENGAGED;
                fieldCol[curFigure->figure.pos[curFigure->state][j].Y  + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][j].X + curFigure->bias.X] = curFigure->figure.color;
            }
            whileLineIsFilled_Update(field, fieldCol, stat);
            return true;
        }

        if(curFigure->figure.pos[curFigure->state][i].Y + 1 + curFigure->bias.Y >= tetrFieldHeight){
            for(j =0; j < BLOCKS_NUMBER; j++){
                field[curFigure->figure.pos[curFigure->state][j].Y  + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][j].X + curFigure->bias.X] = IS_ENGAGED;
                fieldCol[curFigure->figure.pos[curFigure->state][j].Y  + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][j].X + curFigure->bias.X] = curFigure->figure.color;
            }
            whileLineIsFilled_Update(field, fieldCol, stat);
            return true;
        }
    }

    setTextColor(BACK_WHITE);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(curFigure->figure.pos[curFigure->state][i].X + x + curFigure->bias.X, curFigure->figure.pos[curFigure->state][i].Y + y + curFigure->bias.Y);
        putchar(' ');
    }

    curFigure->bias.Y++;

    setTextColor(curFigure->figure.color);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(curFigure->figure.pos[curFigure->state][i].X + x + curFigure->bias.X, curFigure->figure.pos[curFigure->state][i].Y + y + curFigure->bias.Y);
        putchar(' ');
    }
}

void drawFigure(ActingFigure * curFigure){
    int i;
    setTextColor(curFigure->figure.color);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(curFigure->figure.pos[curFigure->state][i].X + fieldX + 1 + curFigure->bias.X, curFigure->figure.pos[curFigure->state][i].Y + fieldY + 1 + curFigure->bias.Y);
        putchar(' ');
    }
}

void tryGoLeft(bool field[tetrFieldHeight][tetrFieldWidth], ActingFigure * curFigure){
    int i, j;
    int x = fieldX + 1, y = fieldY + 1;
    bool check;
    for(i = 0; i < BLOCKS_NUMBER; i++){
        if(field[curFigure->figure.pos[curFigure->state][i].Y + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][i].X - 1 + curFigure->bias.X] == IS_ENGAGED)
            return;
        if(curFigure->figure.pos[curFigure->state][i].X - 1 + curFigure->bias.X < 0) return;
    }

    setTextColor(BACK_WHITE);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(curFigure->figure.pos[curFigure->state][i].X + x + curFigure->bias.X, curFigure->figure.pos[curFigure->state][i].Y + y + curFigure->bias.Y);
        putchar(' ');
    }

    curFigure->bias.X--;

    drawFigure(curFigure);
}

void tryChangeState(bool field[tetrFieldHeight][tetrFieldWidth], ActingFigure * curFigure){
    int i, j;
    int x = fieldX + 1, y = fieldY + 1;
    bool check;
    int nextState = curFigure->state + 1 >= curFigure->figure.statesNum ? 0 : curFigure->state + 1;
    for(i = 0; i < BLOCKS_NUMBER; i++){
        if(field[curFigure->figure.pos[nextState][i].Y + curFigure->bias.Y][curFigure->figure.pos[nextState][i].X + curFigure->bias.X] == IS_ENGAGED)
            return;
        if(curFigure->figure.pos[nextState][i].X + curFigure->bias.X < 0) return;
        if(curFigure->figure.pos[nextState][i].X + curFigure->bias.X >= tetrFieldWidth) return;
        if(curFigure->figure.pos[nextState][i].Y + curFigure->bias.Y < 0) return;
        if(curFigure->figure.pos[nextState][i].Y + curFigure->bias.Y >= tetrFieldHeight) return;
    }

    setTextColor(BACK_WHITE);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(curFigure->figure.pos[curFigure->state][i].X + x + curFigure->bias.X, curFigure->figure.pos[curFigure->state][i].Y + y + curFigure->bias.Y);
        putchar(' ');
    }

    curFigure->state = nextState;

    drawFigure(curFigure);
}

void tryGoRight(bool field[tetrFieldHeight][tetrFieldWidth], ActingFigure * curFigure){
    int i, j;
    int x = fieldX + 1, y = fieldY + 1;
    bool check;
    for(i = 0; i < BLOCKS_NUMBER; i++){
        if(field[curFigure->figure.pos[curFigure->state][i].Y + curFigure->bias.Y][curFigure->figure.pos[curFigure->state][i].X + 1 + curFigure->bias.X] == IS_ENGAGED)
            return;
        if(curFigure->figure.pos[curFigure->state][i].X + 1 + curFigure->bias.X >= tetrFieldWidth) return;
    }

    setTextColor(BACK_WHITE);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(curFigure->figure.pos[curFigure->state][i].X + x + curFigure->bias.X, curFigure->figure.pos[curFigure->state][i].Y + y + curFigure->bias.Y);
        putchar(' ');
    }

    curFigure->bias.X++;

    drawFigure(curFigure);
}

void ReadThread(ToGiveThread * toGet){
    char ch;
    while(true){
        ch = getch();
        WaitForSingleObject(toGet->hMutex, INFINITE);
        switch(ch){
            case LEFT_ARR:
                tryGoLeft(toGet->field, toGet->curFigure);
                ReleaseMutex(toGet->hMutex);
                break;
            case RIGHT_ARR:
                tryGoRight(toGet->field, toGet->curFigure);
                ReleaseMutex(toGet->hMutex);
                break;
            case UP_ARR:
                tryChangeState(toGet->field, toGet->curFigure);
                ReleaseMutex(toGet->hMutex);
                break;
            case DOWN_ARR:
                ReleaseMutex(toGet->hMutex);
                toGet->curFigure->isAccelerating = true;
                Sleep(50);
                toGet->curFigure->isAccelerating = false;
                break;
            default:
                ReleaseMutex(toGet->hMutex);
                break;
        }
    }
}


bool IsGameOver(ActingFigure * curFigure, bool field[tetrFieldHeight][tetrFieldWidth]){
    int i;
    for(i = 0; i < BLOCKS_NUMBER; i++){
        if(field[curFigure->figure.pos[0][i].Y + curFigure->bias.Y][curFigure->figure.pos[0][i].X + curFigure->bias.X] == IS_ENGAGED){
            return true;
        }
    }
}


void updateAds(Stat * stat, ActingFigure * nextFigure){
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);

    moveCursor(fieldX + tetrFieldWidth + 2, fieldY + tetrFieldHeight/5 - 2);
    printf("Lines destroyed: %d", stat->linesDestroyed);

    moveCursor(fieldX + tetrFieldWidth + 2, fieldY + tetrFieldHeight*4/5 - 2);
    printf("Level: %d", stat->level);

    moveCursor(fieldX + tetrFieldWidth + 2, fieldY + tetrFieldHeight*4/5 + 1);
    printf("Score: %d", stat->score);

    moveCursor(fieldX + tetrFieldWidth + 2, fieldY + tetrFieldHeight/3);
    printf("Next figure:");

    setTextColor(BACK_WHITE);
    int i, j;
    for(i = fieldX + tetrFieldWidth + 5; i < fieldX + tetrFieldWidth + 5 + BLOCKS_NUMBER; i++){
        for(j = fieldY + tetrFieldHeight/3 + 2; j < fieldY + tetrFieldHeight/3 + 2 + BLOCKS_NUMBER; j++){
            moveCursor(i, j);
            putchar(' ');
        }
    }

    setTextColor(nextFigure->figure.color);
    for(i = 0; i < BLOCKS_NUMBER; i++){
        moveCursor(nextFigure->figure.pos[0][i].X + fieldX + tetrFieldWidth + 5, nextFigure->figure.pos[0][i].Y + fieldY + tetrFieldHeight/3 + 3);
        putchar(' ');
    }

}

void initFigures(Figure * figures){
    /*I*/
    figures->statesNum = 2;

    figures->pos[0][0].X = 0;
    figures->pos[0][0].Y = 0;

    figures->pos[0][1].X = 1;
    figures->pos[0][1].Y = 0;

    figures->pos[0][2].X = 2;
    figures->pos[0][2].Y = 0;

    figures->pos[0][3].X = 3;
    figures->pos[0][3].Y = 0;
    /*____________*/
    figures->pos[1][0].X = 1;
    figures->pos[1][0].Y = -3;

    figures->pos[1][1].X = 1;
    figures->pos[1][1].Y = -2;

    figures->pos[1][2].X = 1;
    figures->pos[1][2].Y = -1;

    figures->pos[1][3].X = 1;
    figures->pos[1][3].Y = 0;

    figures->color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;

    /*J*/
    (figures + 1)->statesNum = 4;

    (figures + 1)->pos[0][0].X = 0;
    (figures + 1)->pos[0][0].Y = 0;

    (figures + 1)->pos[0][1].X = 0;
    (figures + 1)->pos[0][1].Y = 1;

    (figures + 1)->pos[0][2].X = 1;
    (figures + 1)->pos[0][2].Y = 1;

    (figures + 1)->pos[0][3].X = 2;
    (figures + 1)->pos[0][3].Y = 1;
    /*____________*/
    (figures + 1)->pos[1][0].X = 0;
    (figures + 1)->pos[1][0].Y = -1;

    (figures + 1)->pos[1][1].X = 1;
    (figures + 1)->pos[1][1].Y = -1;

    (figures + 1)->pos[1][2].X = 0;
    (figures + 1)->pos[1][2].Y = 0;

    (figures + 1)->pos[1][3].X = 0;
    (figures + 1)->pos[1][3].Y = 1;
    /*____________*/
    (figures + 1)->pos[2][0].X = 0;
    (figures + 1)->pos[2][0].Y = 0;

    (figures + 1)->pos[2][1].X = 1;
    (figures + 1)->pos[2][1].Y = 0;

    (figures + 1)->pos[2][2].X = 2;
    (figures + 1)->pos[2][2].Y = 0;

    (figures + 1)->pos[2][3].X = 2;
    (figures + 1)->pos[2][3].Y = 1;
    /*____________*/
    (figures + 1)->pos[3][0].X = 2;
    (figures + 1)->pos[3][0].Y = -1;

    (figures + 1)->pos[3][1].X = 2;
    (figures + 1)->pos[3][1].Y = 0;

    (figures + 1)->pos[3][2].X = 2;
    (figures + 1)->pos[3][2].Y = 1;

    (figures + 1)->pos[3][3].X = 1;
    (figures + 1)->pos[3][3].Y = 1;

    (figures + 1)->color = BACKGROUND_BLUE | BACKGROUND_INTENSITY;

    /*O*/
    (figures + 2)->statesNum = 1;

    (figures + 2)->pos[0][0].X = 1;
    (figures + 2)->pos[0][0].Y = 0;

    (figures + 2)->pos[0][1].X = 2;
    (figures + 2)->pos[0][1].Y = 0;

    (figures + 2)->pos[0][2].X = 1;
    (figures + 2)->pos[0][2].Y = 1;

    (figures + 2)->pos[0][3].X = 2;
    (figures + 2)->pos[0][3].Y = 1;

    (figures + 2)->color = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;

    /*T*/
    (figures + 3)->statesNum = 4;

    (figures + 3)->pos[0][0].X = 1;
    (figures + 3)->pos[0][0].Y = 0;

    (figures + 3)->pos[0][1].X = 0;
    (figures + 3)->pos[0][1].Y = 1;

    (figures + 3)->pos[0][2].X = 1;
    (figures + 3)->pos[0][2].Y = 1;

    (figures + 3)->pos[0][3].X = 2;
    (figures + 3)->pos[0][3].Y = 1;
    /*____________*/
    (figures + 3)->pos[1][0].X = 0;
    (figures + 3)->pos[1][0].Y = -1;

    (figures + 3)->pos[1][1].X = 0;
    (figures + 3)->pos[1][1].Y = 0;

    (figures + 3)->pos[1][2].X = 0;
    (figures + 3)->pos[1][2].Y = 1;

    (figures + 3)->pos[1][3].X = 1;
    (figures + 3)->pos[1][3].Y = 0;
    /*____________*/
    (figures + 3)->pos[2][0].X = 0;
    (figures + 3)->pos[2][0].Y = -1;

    (figures + 3)->pos[2][1].X = 1;
    (figures + 3)->pos[2][1].Y = -1;

    (figures + 3)->pos[2][2].X = 2;
    (figures + 3)->pos[2][2].Y = -1;

    (figures + 3)->pos[2][3].X = 1;
    (figures + 3)->pos[2][3].Y = 0;
    /*____________*/
    (figures + 3)->pos[3][0].X = 2;
    (figures + 3)->pos[3][0].Y = -1;

    (figures + 3)->pos[3][1].X = 2;
    (figures + 3)->pos[3][1].Y = 0;

    (figures + 3)->pos[3][2].X = 2;
    (figures + 3)->pos[3][2].Y = 1;

    (figures + 3)->pos[3][3].X = 1;
    (figures + 3)->pos[3][3].Y = 0;

    (figures + 3)->color = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;

    /*Z*/
    (figures + 4)->statesNum = 2;

    (figures + 4)->pos[0][0].X = 0;
    (figures + 4)->pos[0][0].Y = 0;

    (figures + 4)->pos[0][1].X = 1;
    (figures + 4)->pos[0][1].Y = 0;

    (figures + 4)->pos[0][2].X = 1;
    (figures + 4)->pos[0][2].Y = 1;

    (figures + 4)->pos[0][3].X = 2;
    (figures + 4)->pos[0][3].Y = 1;
    /*____________*/
    (figures + 4)->pos[1][0].X = 1;
    (figures + 4)->pos[1][0].Y = 0;

    (figures + 4)->pos[1][1].X = 1;
    (figures + 4)->pos[1][1].Y = 1;

    (figures + 4)->pos[1][2].X = 2;
    (figures + 4)->pos[1][2].Y = -1;

    (figures + 4)->pos[1][3].X = 2;
    (figures + 4)->pos[1][3].Y = 0;

    (figures + 4)->color = BACKGROUND_RED | BACKGROUND_INTENSITY;

    /*S*/
    (figures + 5)->statesNum = 2;

    (figures + 5)->pos[0][0].X = 1;
    (figures + 5)->pos[0][0].Y = 0;

    (figures + 5)->pos[0][1].X = 2;
    (figures + 5)->pos[0][1].Y = 0;

    (figures + 5)->pos[0][2].X = 0;
    (figures + 5)->pos[0][2].Y = 1;

    (figures + 5)->pos[0][3].X = 1;
    (figures + 5)->pos[0][3].Y = 1;
    /*____________*/
    (figures + 5)->pos[1][0].X = 0;
    (figures + 5)->pos[1][0].Y = -1;

    (figures + 5)->pos[1][1].X = 0;
    (figures + 5)->pos[1][1].Y = 0;

    (figures + 5)->pos[1][2].X = 1;
    (figures + 5)->pos[1][2].Y = 0;

    (figures + 5)->pos[1][3].X = 1;
    (figures + 5)->pos[1][3].Y = 1;

    (figures + 5)->color = BACKGROUND_GREEN | BACKGROUND_INTENSITY;

    /*L*/
    (figures + 6)->statesNum = 4;

    (figures + 6)->pos[0][0].X = 2;
    (figures + 6)->pos[0][0].Y = 0;

    (figures + 6)->pos[0][1].X = 0;
    (figures + 6)->pos[0][1].Y = 1;

    (figures + 6)->pos[0][2].X = 1;
    (figures + 6)->pos[0][2].Y = 1;

    (figures + 6)->pos[0][3].X = 2;
    (figures + 6)->pos[0][3].Y = 1;
    /*____________*/
    (figures + 6)->pos[1][0].X = 1;
    (figures + 6)->pos[1][0].Y = -1;

    (figures + 6)->pos[1][1].X = 2;
    (figures + 6)->pos[1][1].Y = -1;

    (figures + 6)->pos[1][2].X = 2;
    (figures + 6)->pos[1][2].Y = 0;

    (figures + 6)->pos[1][3].X = 2;
    (figures + 6)->pos[1][3].Y = 1;
    /*____________*/
    (figures + 6)->pos[2][0].X = 0;
    (figures + 6)->pos[2][0].Y = 1;

    (figures + 6)->pos[2][1].X = 0;
    (figures + 6)->pos[2][1].Y = 0;

    (figures + 6)->pos[2][2].X = 1;
    (figures + 6)->pos[2][2].Y = 0;

    (figures + 6)->pos[2][3].X = 2;
    (figures + 6)->pos[2][3].Y = 0;
    /*____________*/
    (figures + 6)->pos[3][0].X = 0;
    (figures + 6)->pos[3][0].Y = -1;

    (figures + 6)->pos[3][1].X = 0;
    (figures + 6)->pos[3][1].Y = 0;

    (figures + 6)->pos[3][2].X = 0;
    (figures + 6)->pos[3][2].Y = 1;

    (figures + 6)->pos[3][3].X = 1;
    (figures + 6)->pos[3][3].Y = 1;

    (figures + 6)->color = BACKGROUND_GREEN | BACKGROUND_RED ;
}

void initActingFigure(Figure * figures, ActingFigure * actingFigure){
    int actingFigureInd = rand() % FIGURES_NUMBER;

    actingFigure->figure = *(figures + actingFigureInd);
    actingFigure->state = 0;
    actingFigure->bias.X = tetrFieldWidth/2 - 2;
    actingFigure->bias.Y = 0;
    actingFigure->isAccelerating = false;
}

int main()
{
    bool field[tetrFieldHeight][tetrFieldWidth];
    Color fieldCol[tetrFieldHeight][tetrFieldWidth];
    Stat stat;
    char name[50];
    showScore();
    initStat(&stat);
    strcpy(name, stat.nickname);
    initField();
    clearFieldBuf(field, fieldCol);
    srand(time(NULL));
    Figure figures[FIGURES_NUMBER];
    initFigures(figures);
    ActingFigure curFigure, nextFigure;
    HANDLE hMutex = CreateMutex(NULL, false, NULL);
    ToGiveThread toGiveThread = {field, &curFigure, hMutex};
    HANDLE hReadThread = CreateThread(NULL, 0, ReadThread, &toGiveThread, 0, NULL);
    initActingFigure(&figures, &nextFigure);
    strcpy(stat.nickname, name);
    while(true){
        curFigure = nextFigure;
        initActingFigure(&figures, &nextFigure);
        updateAds(&stat, &nextFigure);
        drawFigure(&curFigure);
        if(IsGameOver(&curFigure, field)) break;
        do{
            ReleaseMutex(hMutex);
            int totalTime = 0;
            while(totalTime < 400 - stat.level*27){
                Sleep((totalTime + 50 > 400 - stat.level*27) ? (400 - stat.level*27 - totalTime) : 50);
                totalTime += 50;
                if(curFigure.isAccelerating)
                    break;
            }
            WaitForSingleObject(hMutex, INFINITE);
        }while(!tryGoDown(field, fieldCol, &curFigure, &stat));
    }
    WaitForSingleObject(hMutex, INFINITE);
    CloseHandle(hReadThread);
    CloseHandle(hMutex);
    updateScore(&stat);
    setTextColor(0);
    system("cls");
    showScore();
    setTextColor(FORE_WHITE);
    system("cls");
    return 0;
}
