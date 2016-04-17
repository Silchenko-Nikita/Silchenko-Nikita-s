#ifndef INVESTOR_H_INCLUDED
#define INVESTOR_H_INCLUDED

#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct Investor * Investor_t;

Investor_t Investor_new(xmlDocPtr doc, xmlNodePtr investorNode);
void Investor_update(Investor_t self, const char * key, const char * value); //input is expected to be valid
void Investor_serialize(Investor_t self, xmlDocPtr doc, xmlNodePtr investorNode);
char * Investor_toString(Investor_t self, char * memP);
void Investor_print(Investor_t self);
void Investor_delete(Investor_t self);

#endif // INVESTOR_H_INCLUDED
