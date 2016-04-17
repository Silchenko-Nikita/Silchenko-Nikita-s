#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

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
            for(xmlNodePtr cur1 = cur->xmlChildrenNode; cur1 != NULL; cur1 = cur1->next){
                if(!xmlStrcmp(cur1->name, (const xmlChar *)"name")){
                    xmlChar * name = xmlNodeListGetString(doc, cur1->xmlChildrenNode, 1);
                    strcpy(self->corporationData.name, (const char *) name);
                    free(name);
                }
            }
            strcpy(self->corporationData.country, (const char *) country);
            free(country);
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

//input is expected to be valid
void Investor_update(Investor_t self, const char * key, const char * value){
    if(!strcmp(key, "name")){
        strcpy(self->name, value);
    }else if(!strcmp(key, "surname")){
        strcpy(self->surname, value);
    }else if(!strcmp(key, "corporation_country")){
        strcpy(self->corporationData.country, value);
    }else if(!strcmp(key, "corporation_name")){
        strcpy(self->corporationData.name, value);
    }else if(!strcmp(key, "birthdate")){
        strcpy(self->birthdate, value);
    }else if(!strcmp(key, "year")){
        self->year = atoi(value);
    }else if(!strcmp(key, "investment")){
        self->investment = atof(value);
    }
}

void Investor_serialize(Investor_t self, xmlDocPtr doc, xmlNodePtr investorNode){
    char buff[64];
    xmlNode * corporationNode = NULL;

    xmlNewChild(investorNode, NULL, "name", self->name);
    xmlNewChild(investorNode, NULL, "surname", self->surname);
    xmlNewChild(investorNode, NULL, "birthdate", self->birthdate);

    corporationNode = xmlNewChild(investorNode, NULL, "corporation", NULL);
	xmlNewProp(corporationNode, "country", self->corporationData.country);
	xmlNewChild(corporationNode, NULL, "name", self->corporationData.name);

    sprintf(buff, "%i", self->year);
    xmlNewChild(investorNode, NULL, "year", buff);
    sprintf(buff, "%f", self->investment);
    xmlNewChild(investorNode, NULL, "investment", buff);
}

char * Investor_toString(Investor_t self, char * memP){
    sprintf(memP, "Investor %s %s:\nbirthdate is %s;\ninvested %f million dollars in corporation %s(%s) in %d", self->name, \
           self->surname, self->birthdate, self->investment, self->corporationData.name, self->corporationData.country, self->year);
    return memP;
}

void Investor_print(Investor_t self){
    printf("Investor %s %s:\nbirthdate is %s;\ninvested %f million dollars in corporation %s(%s) in %d", self->name, \
           self->surname, self->birthdate, self->investment, self->corporationData.name, self->corporationData.country, self->year);
}

void Investor_delete(Investor_t self){
    free(self);
}
