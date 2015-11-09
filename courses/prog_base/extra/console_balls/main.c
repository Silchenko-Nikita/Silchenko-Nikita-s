#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "balls.h"
#include "console.h"

int main() {
    const int ballsCount = 20;
    const int SLEEP_TIME = 1;

    /* initialize system objects */
    Ball ballsArr[ballsCount];
    srand(time(NULL));
    initBalls(ballsCount, ballsArr);

    /* infinite loop engine */
    while(1) {
        clearScreen();
        updateBalls(ballsCount, ballsArr);
        drawRect(ballsCount, ballsArr);
        drawBalls(ballsCount, ballsArr);
        tSleep(SLEEP_TIME);
    }
    return EXIT_SUCCESS;
}
