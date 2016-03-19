#include <stdlib.h>

#include "terminal.h"

static struct Terminal{
    unsigned int id;
    Wickets_t wickets;
    Terminal_PrintSettings printSettings
};

typedef struct Terminal Terminal_s;

Terminal_t Terminal_new(unsigned int id, Terminal_PrintSettings printSett, Wickets_t wickets, Terminal_Error * error){
    if(printSett > TERMINAL_PRINT_ALL){
        if(error != NULL) *error = TERMINAL_SETTINGS_ERROR;
        return NULL;
    }

    Terminal_t self = (Terminal_t)malloc(sizeof(Terminal_s));
    if(NULL == self){
        puts("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    self->id = id;
    self->printSettings = printSett;
    self->wickets = wickets;

    if(error != NULL) *error = TERMINAL_OK;
    return self;
}

void Terminal_delete(Terminal_t self){
    if(NULL == self) return;

    free(self);
}

void Terminal_deepDelete(Terminal_t self){
    if(NULL == self) return;

    Wickets_deepDelete(self->wickets);
    free(self);
}

unsigned int Terminal_getId(Terminal_t self){
    return self->id;
}

Terminal_Error Terminal_setPrintSettings(Terminal_t self, Terminal_PrintSettings printSett){
     if(printSett > TERMINAL_PRINT_ALL) return TERMINAL_SETTINGS_ERROR;
     self->printSettings = printSett;
     return TERMINAL_OK;
}

Terminal_PrintSettings Terminal_getPrintSettings(Terminal_t self){
    return self->printSettings;
}

Wickets_t Terminal_setWickets(Terminal_t self, Wickets_t wickets){
    Wickets_t oldWickets = self->wickets;
    self->wickets = wickets;
    return oldWickets;
}

Wickets_t Terminal_getWickets(Terminal_t self){
    return self->wickets;
}

const char * Terminal_errorStr(Terminal_Error error){
    static const char * repr[] = {
        "OK",
        "Settings error"
    };

    return repr[error];
}
