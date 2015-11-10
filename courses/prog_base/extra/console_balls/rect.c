#include <stdio.h>
#include <stdlib.h>

#include "console.h"
#include "rect.h"

void drawRect(Rect *rect) {
    int i;
    setBackgroundColor(64);

    for(i = 0; i < rect->Height - 1; i++) {
        moveCursor(rect->X, rect->Y + i);
        printf(" ");
    }

    for(i = 0; i < rect->Width - 1; i++) {
        moveCursor(rect->X + i, rect->Y);
        printf(" ");
    }

    for(i = 0; i < rect->Height - 1; i++) {
        moveCursor(rect->X + rect->Width - 1, rect->Y + i);
        printf(" ");
    }

    for(i = 0; i < rect->Width; i++) {
        moveCursor(rect->X + i, rect->Y + rect->Height - 1);
        printf(" ");
    }
    clearFormat();
}

void drawRects( int count_rects, Rect *rectsList) {
    int i;

    for(i = 0; i < count_rects; i++) {
        drawRect(rectsList + i);
    }

}
