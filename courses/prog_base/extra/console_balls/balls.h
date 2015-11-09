#ifndef BALLS_H
#define BALLS_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 500

typedef struct Vec2D { float X, Y; } Vec2D;
typedef struct Size2 { float Width, Height; } Size2;
typedef short ConsoleColor;
typedef struct Ball {
    Vec2D Pos;
    Vec2D Vel;
    ConsoleColor Color;
} Ball;

void print(Ball *);
void init(Ball *);
void initBalls(int, Ball *);
void update(Ball *, int, Ball * );
void updateBalls(int, Ball *);
void draw(Ball *);
void drawBalls(int, Ball *);

#endif
