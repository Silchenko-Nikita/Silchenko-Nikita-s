#include <stdlib.h>
#include <stdio.h>

#include "investor.h"

Investor_t Investor_new(){
    return (Investor_t) malloc(sizeof(Investor_s));
}

void Investor_delete(Investor_t self){
    free(self);
}


char * Investor_toString(Investor_t self, char * memP){
    sprintf(memP, "Investor %s %s:\nbirthdate is %s;\ninvested %f million grivnas in %d projects", self->name,
           self->surname, self->birthdate, self->investment, self->projectsNumber);
    return memP;
}

void Investor_print(Investor_t self){
    printf("Investor %s %s:\nbirthdate is %s;\ninvested %f million grivnas in %d projects", self->name,
           self->surname, self->birthdate, self->investment, self->projectsNumber);
}
