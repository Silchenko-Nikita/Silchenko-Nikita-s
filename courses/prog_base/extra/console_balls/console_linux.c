#ifdef __linux__  /* for linux os */

static enum Attribute { RESET, BOLD, FAINT, ITALIC, UNDERLINE, BLINK_SLOW, BLINK_RAPID, REVERSE, CONCEAL, CROSSED };
static enum Color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, DEFAULT };

void moveCursor(int x, int y) {
    printf("\033[%i;%iH", y + 1, x + 1);
}

void setTextColor(short color) {
    printf("\033[%d;%d;%dm", BOLD, BLACK + 30 + (color % 8), DEFAULT + 40);
}

void setBackgroundColor(short color) {
    printf("\033[%d;%d;%dm", BOLD, BLACK + 30, RED);
}

void clearScreen(void) {
    printf("\033[2J");
}

void clearFormat(void) {
    printf("\033[%d;%d;%dm", RESET, DEFAULT + 30, DEFAULT + 40);
}

void tSleep(unsigned int millis) {
    usleep(millis * 10000);
}

#endif
