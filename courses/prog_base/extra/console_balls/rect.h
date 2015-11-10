#ifndef RECT_H
#define RECT_H

typedef struct Rect { int X, Y, Width, Height; } Rect;

void drawRect(Rect *);
void drawRects(int, Rect *);

#endif
