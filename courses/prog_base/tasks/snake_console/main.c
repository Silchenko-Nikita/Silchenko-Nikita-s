#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define LEFT_ARR 'a'
#define RIGHT_ARR 'd'
#define UP_ARR 'w'
#define DOWN_ARR 's'

#define BACK_WHITE BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED
#define FORE_WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define INIT_BLOCKS_NUM 3
#define NUM_OF_BLOCKS_TO_RAISE 7
#define GAME_FIELD_WIDTH 20
#define GAME_FIELD_HEIGHT 20
#define MAX_LEVEL 9

#define sleepTime(level) 300 - 25*level

enum Direction{RIGHT, LEFT, UP, DOWN};

const char * const leadersTableFile = "LeadersTable.txt";
const short fieldX = 23;
const short fieldY = 2;

typedef short Color;
typedef struct Pos{int X, Y} Pos;
typedef struct Snake{Pos blocks[GAME_FIELD_WIDTH*GAME_FIELD_HEIGHT], tail, head; int tailPosInArr, headPosInArr} Snake;
typedef struct Stat{int level, blocksNum, score; char nickname[]} Stat;
typedef struct ToGiveTread{enum Direction direc; bool isChanged, toStopThread} ToGiveTread;

void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
}

void setTextColor(Color color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clearScr(){
    setTextColor(0);
    system("cls");
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
    clearScr();
    drawField(fieldX - 1, fieldY - 1, 40, 4, BACK_WHITE | BACKGROUND_INTENSITY);
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
    moveCursor(fieldX + 2, fieldY - 1);
    printf("Please, enter your nickname:");
    moveCursor(fieldX + 2, fieldY);
    scanf("%s", stat->nickname);

    stat->level = -1;
    do{
        drawField(fieldX - 1, fieldY - 1, 40, 4, BACK_WHITE | BACKGROUND_INTENSITY);
        setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
        moveCursor(fieldX + 2, fieldY - 1);
        printf("Please, enter level(from 1 to 9):");
        moveCursor(fieldX + 2, fieldY);
        scanf("%d", &stat->level);
    }while( stat->level > MAX_LEVEL || stat->level <= 0);
    clearScr();

    stat->score = 0;
    stat->blocksNum = INIT_BLOCKS_NUM;
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

void showLeaderboard(){
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
    drawField(fieldX - 1, 0, 40, 24, BACK_WHITE | BACKGROUND_INTENSITY);
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

void initField(){
    drawField(fieldX - 1, fieldY - 1, GAME_FIELD_WIDTH  + 23, GAME_FIELD_HEIGHT + 2, BACK_WHITE | BACKGROUND_INTENSITY);
    drawField(fieldX, fieldY, GAME_FIELD_WIDTH, GAME_FIELD_HEIGHT, BACK_WHITE);
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);
    moveCursor(fieldX + GAME_FIELD_WIDTH + 2, fieldY + GAME_FIELD_HEIGHT - 1);
    printf("Use \"w, a, s, d\"");
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

void ReadThread(ToGiveTread * got){
    char ch;
    while(true){
        ch = getch();
        if(got->toStopThread) return;
        switch(ch){
            case LEFT_ARR:
                if(got->direc != RIGHT && got->direc != LEFT && !got->isChanged){
                    got->direc = LEFT;
                    got->isChanged = true;
                }
                break;
            case RIGHT_ARR:
                if(got->direc != RIGHT && got->direc != LEFT && !got->isChanged){
                    got->direc = RIGHT;
                    got->isChanged = true;
                }
                break;
            case UP_ARR:
                if(got->direc != UP && got->direc != DOWN && !got->isChanged){
                    got->direc = UP;
                    got->isChanged = true;
                }
                break;
            case DOWN_ARR:
                if(got->direc != UP && got->direc != DOWN && !got->isChanged){
                    got->direc = DOWN;
                    got->isChanged = true;
                }
                break;
            default:
                break;
        }
    }
}

void drawBlock(int x, int y, Color color){
    setTextColor(color);
    moveCursor(x, y);
    putchar(' ');
}

void initSnake(Snake * snake){
    snake->head.X = snake->blocks[0].X = GAME_FIELD_WIDTH/2;
    snake->head.Y = snake->blocks[0].Y = GAME_FIELD_HEIGHT/2;
    snake->blocks[1].X = GAME_FIELD_WIDTH/2 - 1;
    snake->blocks[1].Y = GAME_FIELD_HEIGHT/2;
    snake->tail.X = snake->blocks[2].X = GAME_FIELD_WIDTH/2 - 2;
    snake->tail.Y = snake->blocks[2].Y = GAME_FIELD_HEIGHT/2;
    snake->tailPosInArr = 2;
    snake->headPosInArr = 0;

    drawBlock(snake->head.X + fieldX, snake->head.Y + fieldY, BACKGROUND_BLUE);

    int i;
    for(i = 1; i < INIT_BLOCKS_NUM; i++){
        drawBlock(snake->blocks[i].X + fieldX, snake->blocks[i].Y + fieldY, BACKGROUND_BLUE | BACKGROUND_GREEN);
    }
}

bool updateSnake(Snake * snake, Pos * food, ToGiveTread * toGiveTread, Stat * stat){
    drawBlock(snake->tail.X  + fieldX, snake->tail.Y + fieldY, BACK_WHITE);
    drawBlock(snake->head.X + fieldX, snake->head.Y + fieldY, BACKGROUND_BLUE | BACKGROUND_GREEN);

    switch(toGiveTread->direc){
        case RIGHT:
            snake->head.X++;
            if(snake->head.X >= GAME_FIELD_WIDTH)
                return true;
            break;
        case LEFT:
            snake->head.X--;
            if(snake->head.X < 0)
                return true;
            break;
        case UP:
            snake->head.Y--;
            if(snake->head.Y < 0)
                return true;
            break;
        case DOWN:
            snake->head.Y++;
            if(snake->head.Y >= GAME_FIELD_HEIGHT)
                return true;
            break;
    }
    toGiveTread->isChanged = false;

    drawBlock(snake->head.X + fieldX, snake->head.Y + fieldY, BACKGROUND_BLUE);

    bool toInitFood = false;
    if(snake->head.X == food->X && snake->head.Y == food->Y){
        toInitFood = true;
        stat->blocksNum++;
        stat->score += stat->level;
        if((stat->blocksNum - INIT_BLOCKS_NUM) % NUM_OF_BLOCKS_TO_RAISE == 0 && stat->level < MAX_LEVEL)
            stat->level++;
        updateAds(stat);

        int i;
        for(i = stat->blocksNum - 1; i > snake->headPosInArr; i--){
            if(i == snake->tailPosInArr) snake->tailPosInArr++;
            snake->blocks[i] = snake->blocks[i - 1];
        }
        snake->blocks[snake->headPosInArr] = snake->head;
    }else{
        snake->headPosInArr = snake->tailPosInArr;
        snake->blocks[snake->tailPosInArr--] = snake->head;
        if(snake->tailPosInArr < 0)
            snake->tailPosInArr = stat->blocksNum - 1;
        snake->tail = snake->blocks[snake->tailPosInArr];
    }

    if(toInitFood) initFood(snake, food, stat);
    int i;
    for(i = 0; i < stat->blocksNum; i++){
        if(i == snake->headPosInArr) continue;
        if(snake->head.X == snake->blocks[i].X && snake->head.Y == snake->blocks[i].Y)
            return true;
    }
    return false;
}

void initFood(Snake * snake, Pos * food, Stat * stat){
    int x, y, i, check;
    do{
        check = true;
        x = rand() % GAME_FIELD_WIDTH;
        y = rand() % GAME_FIELD_HEIGHT;
        for(i = 0; i < stat->blocksNum; i++){
           if(x == snake->blocks[i].X && y == snake->blocks[i].Y){
                check = false;
                break;
           }
        }
    }while(!check);
    food->X = x;
    food->Y = y;
    drawBlock(x + fieldX, y + fieldY, BACKGROUND_RED | BACKGROUND_GREEN);
}

void updateAds(Stat * stat){
    setTextColor(BACK_WHITE | BACKGROUND_INTENSITY);

    moveCursor(fieldX + GAME_FIELD_WIDTH + 1, fieldY + GAME_FIELD_HEIGHT/4 - 2);
    printf("Number of blocks: %d", stat->blocksNum);

    moveCursor(fieldX + GAME_FIELD_WIDTH + 1, fieldY + GAME_FIELD_HEIGHT/2 - 2);
    printf("Level: %d", stat->level);

    moveCursor(fieldX + GAME_FIELD_WIDTH + 1, fieldY + GAME_FIELD_HEIGHT*3/4 - 2);
    printf("Score: %d", stat->score);

}

int main()
{
    srand(time(NULL));
    Stat stat;
    char name[50];
    Snake snake;
    Pos food;
    ToGiveTread toGiveThread = {RIGHT, false, false};
    showLeaderboard();
    initStat(&stat);
    strcpy(name, stat.nickname);
    HANDLE hReadThread = CreateThread(NULL, 0, ReadThread, &toGiveThread, 0, NULL);
    strcpy(stat.nickname, name);
    initField();
    initSnake(&snake);
    initFood(&snake, &food, &stat);
    updateAds(&stat);
    do{
        Sleep(sleepTime(stat.level));
    }while(!updateSnake(&snake, &food, &toGiveThread, &stat));
    toGiveThread.toStopThread = true;
    clearScr();
    updateScore(&stat);
    CloseHandle(hReadThread);
    showLeaderboard();
    return 0;
}
