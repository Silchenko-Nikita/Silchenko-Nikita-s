#ifndef CONSOLE_H
#define CONSOLE_H

#define COLS 79
#define ROWS 24

typedef struct Cell{char text; short atr;} Cell;
typedef struct dBuff{Cell buff1[ROWS][COLS], buff2[ROWS][COLS];} dBuff;

void setTextColor(short);
void moveCursor(int, int);
void clearBuff(Cell buff[ROWS][COLS]);
void clearBuffs(dBuff*);
void clearFormat(void);
void switchBuffs(dBuff*);

#endif
