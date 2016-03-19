#include <stdlib.h>

#include "wickets.h"
#include "list.h"

static struct Wickets{
    List_t passersList;
};

typedef struct Wickets Wickets_s;

Wickets_t Wickets_new(void){
    Wickets_t self = (Wickets_t)malloc(sizeof(Wickets_s));
    if(NULL == self){
        puts("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    self->passersList = List_new();

    return self;
}

void Wickets_delete(Wickets_t self){
    if(NULL == self) return;

    List_delete(self->passersList);
    free(self);
}

void Wickets_deepDelete(Wickets_t self){
    if(NULL == self) return;

    List_deepDelete(self->passersList, Person_delete);
    free(self);
}

void Wickets_addPasser(Wickets_t self, Person_t passer){
    List_append(self->passersList, passer);
}

Person_t Wickets_getPasser(Wickets_t self, unsigned int index, Wickets_Error * error){
    if(index >= List_getSize(self->passersList)){
        if(error != NULL) *error = WICKETS_INDEX_ERROR;
        return NULL;
    }

    return List_get(self->passersList, index, NULL);
}

unsigned int Wickets_getPassersNumber(Wickets_t self){
    return List_getSize(self->passersList);
}

Wickets_Error Wickets_emulatePasses(Wickets_t self, unsigned int hours){
    if(hours > 10) return WICKETS_EMULATION_ERROR;

    for(int i = 0; i < hours; i++){
        int passersNum = rand() % 5;
        for(int i = 0; i < passersNum; i++){
            Wickets_addPasser(self, Person_newRandom());
        }
    }
}

const char * Wickets_errorStr(Wickets_Error error){
    static const char * repr[] = {
        "OK",
        "Index error",
        "Emulation error"
    };

    return repr[error];
}
