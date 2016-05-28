#include <stdlib.h>
#include <stdio.h>

#include "investor.h"

Investor_t Investor_new(){
    return (Investor_t) malloc(sizeof(Investor_s));
}

void Investor_delete(Investor_t self){
    free(self);
}

void Investor_fromXmlNode(Investor_t self, xmlDocPtr doc, xmlNodePtr investorNode){
    for (xmlNodePtr cur = investorNode->xmlChildrenNode; cur != NULL; cur = cur->next) {
        if (!xmlStrcmp(cur->name, (const xmlChar *)"id")) {
            xmlChar * idStr = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            self->id = atoi((const char *) idStr);
            free(idStr);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"name")) {
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
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"investment")) {
            xmlChar * yearStr = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            self->investment = atof((const char *) yearStr);
            free(yearStr);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"projectsNumber")) {
            xmlChar * investmentStr = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            self->projectsNumber = atoi((const char *) investmentStr);
            free(investmentStr);
        }
    }
}

void Investor_toXmlNode(Investor_t self, xmlDocPtr doc, xmlNodePtr parentNode){
    char buff[64];
    xmlNodePtr investorNode = NULL;

    if(parentNode != NULL){
        investorNode = xmlNewChild(parentNode, NULL, "investor", NULL);
    }else{
        investorNode = xmlNewNode(NULL, "investor");
        xmlDocSetRootElement(doc, investorNode);
    }

    sprintf(buff, "%i", self->id);
    xmlNewChild(investorNode, NULL, "id", buff);

    xmlNewChild(investorNode, NULL, "name", self->name);
    xmlNewChild(investorNode, NULL, "surname", self->surname);
    xmlNewChild(investorNode, NULL, "birthdate", self->birthdate);

    sprintf(buff, "%f", self->investment);
    xmlNewChild(investorNode, NULL, "investment", buff);

    sprintf(buff, "%i", self->projectsNumber);
    xmlNewChild(investorNode, NULL, "projectsNumber", buff);
}

char * Investor_toXmlStr(Investor_t self, char * mem_p){
    xmlDoc * doc = NULL;
	xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");

	Investor_toXmlNode(self, doc, NULL);

	xmlDocDumpMemory(doc, &xmlStr, NULL);
	strcpy(mem_p, xmlStr);

	free(xmlStr);
    xmlFreeDoc(doc);
    return mem_p;
}

void Investor_update(Investor_t self, const char * key, const char * value){
    if(!strcmp(key, "name")){
        strcpy(self->name, value);
    }else if(!strcmp(key, "surname")){
        strcpy(self->surname, value);
    }else if(!strcmp(key, "birthdate")){
        strcpy(self->birthdate, value);
    }else if(!strcmp(key, "projectsNumber")){
        self->projectsNumber = atoi(value);
    }else if(!strcmp(key, "investment")){
        self->investment = atof(value);
    }
}

char * Investor_toString(Investor_t self, char * mem_p){
    sprintf(mem_p, "Investor %s %s:\nbirthdate is %s;\ninvested %f grivnas in %d projects", self->name,
           self->surname, self->birthdate, self->investment, self->projectsNumber);
    return mem_p;
}

void Investor_print(Investor_t self){
    printf("Investor %s %s:\nbirthdate is %s;\ninvested %f grivnas in %d projects", self->name,
           self->surname, self->birthdate, self->investment, self->projectsNumber);
}
