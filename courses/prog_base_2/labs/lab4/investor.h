#ifndef INVESTOR_H_INCLUDED
#define INVESTOR_H_INCLUDED

#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct Investor{
    char name[64];
    char surname[64];
    char birthdate[16];
    float investment;
    unsigned int projectsNumber;
} Investor_s;

typedef Investor_s * Investor_t;

Investor_t Investor_new();
void Investor_delete(Investor_t self);

void Investor_fromXmlNode(Investor_t self, xmlDocPtr doc, xmlNodePtr investorNode);
void Investor_toXmlNode(Investor_t self, xmlDocPtr doc, xmlNodePtr investorNode);
void Investor_update(Investor_t self, const char * key, const char * value);

char * Investor_toString(Investor_t self, char * memP);
void Investor_print(Investor_t self);

#endif // INVESTOR_H_INCLUDED
