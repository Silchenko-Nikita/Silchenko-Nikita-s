#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "investor.h"

static struct Corporation{
    char country[24];
    char name[24]
};

typedef struct Corporation Corporation_s;

static struct Investor{
    char name[24];
    char surname[24];
    char birthdate[16];

    Corporation_s corporationData;

    unsigned int year;
    float investment
};

typedef struct Investor Investor_s;


Investor_t Investor_new(xmlDocPtr doc, xmlNodePtr investorNode){
    Investor_t self = (Investor_t)malloc(sizeof(Investor_s));

    int k = 0;
    for (xmlNodePtr cur = investorNode->xmlChildrenNode; cur != NULL; cur = cur->next) {
        if (!xmlStrcmp(cur->name, (const xmlChar *)"name")) {
            xmlChar * name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->name, (const char *) name);
            free(name);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"surname")) {
            xmlChar * surname = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->surname, (const char *) surname);
            free(surname);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"birthdate")) {
            xmlChar * birthdate = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->birthdate, (const char *) birthdate);
            free(birthdate);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"corporation")) {
            xmlChar * country = xmlGetProp(cur, "country");
            xmlChar * name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->corporationData.country, (const char *) country);
            strcpy(self->corporationData.name, (const char *) name);
            free(country);
            free(name);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"corporation")) {
            xmlChar * country = xmlGetProp(cur, "country");
            xmlChar * name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->corporationData.country, (const char *) country);
            strcpy(self->corporationData.name, (const char *) name);
            free(country);
            free(name);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"year")) {
            xmlChar * yearStr = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            self->year = atoi((const char *) yearStr);
            free(yearStr);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"investment")) {
            xmlChar * investmentStr = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            self->investment = atof((const char *) investmentStr);
            free(investmentStr);
        }
    }

    return self;
}

void Investor_print(Investor_t self){
    printf("Investor %s %s:\nbirthdate is %s;\ninvested %f million dollars in corporation %s(%s) in %d\n\n", self->name, \
           self->surname, self->birthdate, self->investment, self->corporationData.name, self->corporationData.country, self->year);
}

void Investor_delete(Investor_t self){
    free(self);
}
