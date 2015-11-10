#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "balls.h"
#include "console.h"
#include "rect.h"

void init(Ball *ball, int count_rects, Rect* rectsList) {
    const int MAX_VEL = 30;
    const int MIN_VEL = 2;
    int cx, cy, check;
    Rect * rect;
    int i;
    do{
        check = 0;
        ball->Pos.X = rand() % SCREEN_WIDTH;
        ball->Pos.Y = rand() % SCREEN_HEIGHT;
        cx = ball->Pos.X / SCREEN_WIDTH * CONSOLE_COLUMNS;
        cy = ball->Pos.Y / SCREEN_HEIGHT * CONSOLE_ROWS;
        for(i = 0; i < count_rects; i++){
            rect = rectsList + i;
            if(cx >= rect->X && cx < rect->X + rect->Width && cy >= rect->Y && cy < rect->Y + rect->Height) check = 1;
        }
   } while(check);

    ball->Vel.X = rand() % MAX_VEL - MAX_VEL / 2;
    if(abs(ball->Vel.X) < MIN_VEL)
        ball->Vel.X = MIN_VEL;
    ball->Vel.Y = rand() % MAX_VEL - MAX_VEL / 2;
    if(abs(ball->Vel.Y) < MIN_VEL)
        ball->Vel.Y = MIN_VEL;
    ball->Color = (rand() % 7) + 1;
}

void update(Ball *ball, int count_balls, Ball * ballsList, int count_rects, Rect *rectsList) {
    int i;
    Ball * pointer;
    float someX, someY;
    int cx, cy, cx1, cy1, cxB, cyB, isRepulsed, bool1, bool2, bool3, bool4;
    isRepulsed = 0;

    for(i = 0; i < count_balls ; i++) {
        pointer = (ballsList + i);
        if(pointer == ball) continue;
        if(abs((pointer->Pos.X) - (ball->Pos.X)) <= 15 && abs((pointer->Pos.Y) - (ball->Pos.Y)) <= 15){
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

    cx = ball->Pos.X / SCREEN_WIDTH * CONSOLE_COLUMNS;
    cy = ball->Pos.Y / SCREEN_HEIGHT * CONSOLE_ROWS;
    cx1 = (ball->Pos.X + ball->Vel.X) / SCREEN_WIDTH * CONSOLE_COLUMNS;
    cy1 = (ball->Pos.Y + ball->Vel.Y) / SCREEN_HEIGHT * CONSOLE_ROWS;
    Rect *rect;

    for(i = 0; i < count_rects; i++){
        rect = rectsList + i;

        if(((cx < rect->X) && (cx1 >= rect->X )) || (cx >= (rect->X + rect->Width) && cx1 < (rect->X + rect->Width))) {
            if((cy1 >= rect->Y - 1  && cy1 <= rect->Y + rect->Height)){
                ball->Vel.X = -(ball->Vel.X);
                ball->Pos.X += ball->Vel.X;
                isRepulsed = 1;
            }
        }
        if(((cy < rect->Y) && (cy1 >= rect->Y )) || (cy >= (rect->Y + rect->Height) && cy1 < (rect->Y + rect->Height))) {
            if(cx1 >= rect->X - 1 && cx1 <= rect->X + rect->Width ){
                ball->Vel.Y = -(ball->Vel.Y);
                ball->Pos.Y += ball->Vel.Y;
                isRepulsed = 1;
            }
        }
        /* Щоб вже точно не було курйозів. Погано зробив, але цей рект мене зовсім змучив*/
        bool1 = (cx1 >= rect->X  && cx1 < (rect->X + rect->Width));
        bool2 = ((cy1 >= rect->Y) && cy1 < (rect->Y + rect->Height));
        if(!isRepulsed && bool1 && bool2) {
                if(bool1 && bool2 && cx <= rect->X + 1){
                    ball->Pos.X -= 25;
                }
                if(bool1 && bool2 && cx >= rect->X + rect->Width - 2){
                    ball->Pos.X += 25;
                }
                if(bool1 && bool2 && cy <= rect->Y + 1){
                    ball->Pos.Y -= 25;
                }
                if(bool1 && bool2 && cy >= rect->Y + rect->Height - 2){
                    ball->Pos.Y += 25;
                }
                ball->Vel.X = -ball->Vel.X;
                ball->Pos.X += ball->Vel.X;
                ball->Vel.Y = -ball->Vel.Y;
                ball->Pos.Y += ball->Vel.Y;
        }
    }

    ball->Pos.X += ball->Vel.X;
    ball->Pos.Y += ball->Vel.Y;

    if(ball->Pos.X >= SCREEN_WIDTH - 1 || ball->Pos.X <= 0) {
        ball->Vel.X = -ball->Vel.X;
        ball->Pos.X += ball->Vel.X;
    }
    if(ball->Pos.Y >= SCREEN_HEIGHT - 1 || ball->Pos.Y <= 0) {
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

void initAll(int count_balls, Ball * ballsList, int count_rects, Rect* rectsList) {
    int i;
    for(i = 0; i < count_balls; i++) {
        init(ballsList + i, count_rects, rectsList);
    }
}

void updateBalls(int count_balls, Ball * ballsList, int count_rects, Rect *rectsList) {
    int i;
    for(i = 0; i < count_balls; i++) {
        update(ballsList + i, count_balls, ballsList, count_rects, rectsList);
    }

}

void drawBalls(int count_balls, Ball * ballsList) {
    int i;
    for(i = 0; i < count_balls; i++) {
        draw(ballsList + i);
    }
}



void print(Ball *b) {
    printf("Position: %f %f\n", b->Pos.X, b->Pos.Y);
    printf("Velocity: %f %f\n", b->Vel.X, b->Vel.Y);
}
