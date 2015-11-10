#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "balls.h"
#include "console.h"
#include "rect.h"


int main() {
    const int ballsCount = 20;
    const int rectsCount = 2;
    const int SLEEP_TIME = 1;

    /* initialize system objects */
    Ball ballsArr[ballsCount];
    srand(time(NULL));

    Rect rects[rectsCount];
    rects[0].X = 10;
    rects[0].Y = 5;
    rects[0].Width = 25;
    rects[0].Height = 10;

    rects[1].X = 50;
    rects[1].Y = 5;
    rects[1].Width = 15;
    rects[1].Height = 5;

    initAll(ballsCount, ballsArr, rectsCount, rects);

    /* infinite loop engine */
    while(1) {
        clearScreen();
        drawRects(2, rects);
        updateBalls(ballsCount, ballsArr, rectsCount, rects);
        drawBalls(ballsCount, ballsArr);
        tSleep(SLEEP_TIME);
    }
    return EXIT_SUCCESS;
}
