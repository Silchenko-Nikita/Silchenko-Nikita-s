#ifndef WICKET_H_INCLUDED
#define WICKET_H_INCLUDED

#include <stdbool.h>

#include "person.h"

typedef struct Wickets * Wickets_t;

typedef enum {
    WICKETS_OK,
    WICKETS_INDEX_ERROR,
    WICKETS_EMULATION_ERROR
} Wickets_Error;

Wickets_t Wickets_new(void);
void Wickets_delete(Wickets_t self);
void Wickets_deepDelete(Wickets_t self);
void Wickets_addPasser(Wickets_t self, Person_t passer);
Person_t Wickets_getPasser(Wickets_t self, unsigned int index, Wickets_Error * error);
unsigned int Wickets_getPassersNumber(Wickets_t self);
Wickets_Error Wickets_emulatePasses(Wickets_t self, unsigned int hours);
const char * Wickets_errorStr(Wickets_Error error);

#endif // WICKET_H_INCLUDED
