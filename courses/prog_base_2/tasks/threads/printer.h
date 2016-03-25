#ifndef PRINTER_H_INCLUDED
#define PRINTER_H_INCLUDED

#ifdef _WIN32
    #include <windows.h>
#elif defined __linux__
    #include <unistd.h>
#endif // _WIN32

#include "thread.h"
#include "mutex.h"


typedef struct Printer * Printer_t;

Printer_t Printer_new(const char * str, mutex_t * mutex);
void Printer_startPrintLoop(Printer_t self);
void Printer_join(Printer_t self);
void Printer_delete(Printer_t self);

#endif // PRINTER_H_INCLUDED
