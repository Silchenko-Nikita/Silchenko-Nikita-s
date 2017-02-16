#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_COLUMNS 80
#define CONSOLE_ROWS 24

void setTextColor(short);
void moveCursor(int, int);
void clearScreen(void);
void clearFormat(void);
void tSleep(unsigned int millis);
void setBackgroundColor(short);

#endif
