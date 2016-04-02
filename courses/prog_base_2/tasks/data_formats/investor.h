#ifndef INVESTOR_H_INCLUDED
#define INVESTOR_H_INCLUDED

#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct Investor * Investor_t;

Investor_t Investor_new(xmlDocPtr doc, xmlNodePtr investorNode);
void Investor_print(Investor_t self);
void Investor_delete(Investor_t self);

#endif // INVESTOR_H_INCLUDED
