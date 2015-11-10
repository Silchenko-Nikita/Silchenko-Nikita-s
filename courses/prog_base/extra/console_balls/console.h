#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_COLUMNS 80
#define CONSOLE_ROWS 25
#define PARTITION_X 15
#define PARTITION_Y 8
#define PARTITION_WIDTH 40
#define PARTITION_HEIGHT 10

void setTextColor(short);
void moveCursor(int, int);
void clearScreen(void);
void clearFormat(void);
void tSleep(unsigned int millis);

#endif
