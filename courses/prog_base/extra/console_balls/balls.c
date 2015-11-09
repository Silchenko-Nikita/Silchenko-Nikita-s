#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "balls.h"
#include "console.h"

void init(Ball *ball) {
    const int MAX_VEL = 30;
    const int MIN_VEL = 2;
    int cx, cy;

    do{
    ball->Pos.X = rand() % SCREEN_WIDTH;
    ball->Pos.Y = rand() % SCREEN_HEIGHT;
    cx = ball->Pos.X / SCREEN_WIDTH * CONSOLE_COLUMNS;
    cy = ball->Pos.Y / SCREEN_HEIGHT * CONSOLE_ROWS;
    } while(cx >= PARTITION_X && cx <= PARTITION_X + PARTITION_WIDTH && cy >= PARTITION_Y && cy <= PARTITION_Y + PARTITION_HEIGHT);

    ball->Vel.X = rand() % MAX_VEL - MAX_VEL / 2;
    if(abs(ball->Vel.X) < MIN_VEL)
        ball->Vel.X = MIN_VEL;
    ball->Vel.Y = rand() % MAX_VEL - MAX_VEL / 2;
    if(abs(ball->Vel.Y) < MIN_VEL)
        ball->Vel.Y = MIN_VEL;
    ball->Color = (rand() % 7) + 1;
}

void update(Ball *ball, int count_balls, Ball * ballsList) {
    int i;
    Ball * pointer;
    float someX, someY;
    int cx, cy, cx1, cy1, isRepulsed;
    isRepulsed = 0;

    cx = ball->Pos.X / SCREEN_WIDTH * CONSOLE_COLUMNS;
    cy = ball->Pos.Y / SCREEN_HEIGHT * CONSOLE_ROWS;
    cx1 = (ball->Pos.X + ball->Vel.X) / SCREEN_WIDTH * CONSOLE_COLUMNS;
    cy1 = (ball->Pos.Y + ball->Vel.Y) / SCREEN_HEIGHT * CONSOLE_ROWS;

    if(((cx <= (PARTITION_X - 1)) && (cx1 > (PARTITION_X - 1))) || (cx >= (PARTITION_X + PARTITION_WIDTH) && cx1 < (PARTITION_X + PARTITION_WIDTH))) {
        if((cy >= PARTITION_Y - 1 && cy <= PARTITION_Y + PARTITION_HEIGHT)){
            ball->Vel.X = -ball->Vel.X;
            ball->Pos.X += ball->Vel.X;
            isRepulsed = 1;
        }
    }
    if(((cy <= (PARTITION_Y - 1)) && (cy1 > (PARTITION_Y - 1))) || (cy >= (PARTITION_Y + PARTITION_HEIGHT) && cy1 < (PARTITION_Y + PARTITION_HEIGHT))) {
        if(cx >= PARTITION_X - 1 && cx <= PARTITION_X + PARTITION_WIDTH ){
            ball->Vel.Y = -ball->Vel.Y;
            ball->Pos.Y += ball->Vel.Y;
            isRepulsed = 1;
        }
    }

    for(i = 0; i < count_balls && !isRepulsed; i++) {
        pointer = (ballsList + i);
        if(pointer == ball) continue;
        if(abs((pointer->Pos.X) - (ball->Pos.X)) < 15 && abs((pointer->Pos.Y) - (ball->Pos.Y)) < 15){
            someX = ball->Vel.X;
            someY = ball->Vel.Y;

            ball->Vel.X = pointer->Vel.X;
            ball->Vel.Y = pointer->Vel.Y;
            pointer->Vel.X = someX;
            pointer->Vel.Y = someY;

            ball->Pos.X += ball->Vel.X;
            ball->Pos.Y += ball->Vel.Y;
            pointer->Pos.X += ball->Vel.X;
            pointer->Pos.Y += ball->Vel.Y;
        }
    }

    ball->Pos.X += ball->Vel.X;
    ball->Pos.Y += ball->Vel.Y;

    if(ball->Pos.X >= SCREEN_WIDTH || ball->Pos.X <= 0) {
        ball->Vel.X = -ball->Vel.X;
        ball->Pos.X += ball->Vel.X;
    }
    if(ball->Pos.Y >= SCREEN_HEIGHT || ball->Pos.Y <= 0) {
        ball->Vel.Y = -ball->Vel.Y;
        ball->Pos.Y += ball->Vel.Y;
    }
}

void draw(Ball *b) {
    /* from floating screen to console coordinates */
    int cx = b->Pos.X / SCREEN_WIDTH * CONSOLE_COLUMNS;
    int cy = b->Pos.Y / SCREEN_HEIGHT * CONSOLE_ROWS;
    setTextColor(b->Color);
    moveCursor(cx, cy);
    printf("O");
    clearFormat();
}

void initBalls(int count_balls, Ball * ballsList) {
    int i;
    for(i = 0; i < count_balls; i++) {
        init(&ballsList[i]);
    }
}

void updateBalls(int count_balls, Ball * ballsList) {
    int i;
    for(i = 0; i < count_balls; i++) {
        update(&ballsList[i], count_balls, ballsList);
    }
}

void drawBalls(int count_balls, Ball * ballsList) {
    int i;
    for(i = 0; i < count_balls; i++) {
        draw(&ballsList[i]);
    }
}

void drawRect(int count_balls, Ball * ballsList) {
    int i;
    setBackgroundColor(64);

    for(i = 0; i < PARTITION_HEIGHT - 1; i++) {
        moveCursor(PARTITION_X, PARTITION_Y + i);
        printf(" ");
    }

    for(i = 0; i < PARTITION_WIDTH - 1; i++) {
        moveCursor(PARTITION_X + i, PARTITION_Y);
        printf(" ");
    }

    for(i = 0; i < PARTITION_HEIGHT - 1; i++) {
        moveCursor(PARTITION_X + PARTITION_WIDTH - 1, PARTITION_Y + i);
        printf(" ");
    }

    for(i = 0; i < PARTITION_WIDTH; i++) {
        moveCursor(PARTITION_X + i, PARTITION_Y + PARTITION_HEIGHT - 1);
        printf(" ");
    }
}

void print(Ball *b) {
    printf("Position: %f %f\n", b->Pos.X, b->Pos.Y);
    printf("Velocity: %f %f\n", b->Vel.X, b->Vel.Y);
}
