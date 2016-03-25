#include <strings.h>

#include "printer.h"

static struct Printer{
    char str[64];
    mutex_t * mutex;
    thread_t * thread
};

static void * startPrintLoop(Printer_t self){
    for(int i = 0; i < 5; i++){
        mutex_lock(self->mutex);
        for(int i = 0; i < 10; i++){
            puts(self->str);
        }
        mutex_unlock(self->mutex);
    }
}

Printer_t Printer_new(const char * str, mutex_t * mutex){
    Printer_t self = (Printer_t)malloc(sizeof(struct Printer));
    if(NULL == self){
        puts("error allocating memory");
        exit(1);
    }

    strcpy(self->str, str);
    self->mutex = mutex;
    self->thread = NULL;

    return self;
}

void Printer_startPrintLoop(Printer_t self){
    if(self->thread != NULL)
        thread_free(self->thread);

    self->thread = thread_create_args(startPrintLoop, self);
}

Printer_join(Printer_t self){
    if(self->thread != NULL)
        thread_join(self->thread);
}

void Printer_delete(Printer_t self){
    mutex_free(self->mutex);
    thread_free(self->thread);
    free(self);

}
