#include <stdio.h>
#include <stdlib.h>

#include "printer.h"

int main()
{
    mutex_t * mutex = mutex_new();
    Printer_t printers[3];
    for(int i = 0; i < 3; i++){
        char str[16];
        printers[i] = Printer_new(itoa(i + 1, str, 10), mutex);
    }

    for(int i = 0; i < 3; i++){
        Printer_startPrintLoop(printers[i]);
    }

    for(int i = 0; i < 3; i++){
        Printer_join(printers[i]);
    }

    for(int i = 0; i < 3; i++){
        Printer_delete(printers[i]);
    }
    mutex_free(mutex);
    return 0;
}
