#ifndef INVESTOR_H_INCLUDED
#define INVESTOR_H_INCLUDED

typedef struct Investor{
    int id;
    char name[64];
    char surname[64];
    char birthdate[16];
    float investment;
    unsigned int projectsNumber
} Investor_s;

typedef Investor_s * Investor_t;

Investor_t Investor_new();
void Investor_delete(Investor_t self);

char * Investor_toString(Investor_t self, char * memP);
void Investor_print(Investor_t self);

#endif // INVESTOR_H_INCLUDED
