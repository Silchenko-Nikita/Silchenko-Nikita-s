#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdlib.h>

#include "data_handler.h"

#include "list.h"

struct DataHandler{
    List_t investorsList;
};

static const char * const invesorsXmlPass = "data/investors.xml";

DataHandler_t DataHandler_new(){
    DataHandler_t self = (DataHandler_t) malloc(sizeof(DataHandler_s));
    self->investorsList = List_new();
    return self;
}

void DataHandler_delete(DataHandler_t self){
    List_deepDelete(self->investorsList, Investor_delete);
    free(self);
}

void DataHandler_deserializeInvestors(DataHandler_t self){
    xmlDocPtr doc = NULL;
    xmlNodePtr root_element = NULL;

    if ((doc = xmlParseFile(invesorsXmlPass)) == NULL){
        fprintf(stderr, "error: could not parse file %s\n", invesorsXmlPass);
    }

    if ((root_element = xmlDocGetRootElement(doc)) == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
    }

    if(xmlStrcmp(root_element->name, (const xmlChar *)"investors")){
        xmlFreeDoc(doc);
    }

    for(xmlNodePtr cur = root_element->xmlChildrenNode; cur != NULL; cur = cur->next){
        if (!xmlStrcmp(cur->name, (const xmlChar *)"investor")) {
            Investor_t inv = Investor_new();
            Investor_fromXmlNode(inv, doc, cur);
            List_append(self->investorsList, inv);
        }
    }

    xmlFreeDoc(doc);
}

void DataHandler_serializeInvestors(DataHandler_t self){
    xmlDoc * doc = NULL;
	xmlNode * root_element = NULL;

	doc = xmlNewDoc("1.0");
	root_element = xmlNewNode(NULL, "investors");
	xmlDocSetRootElement(doc, root_element);

	int investorsNum = List_getSize(self->investorsList);
	for(int i = 0; i < investorsNum; i++){
        Investor_toXmlNode(List_get(self->investorsList, i, NULL), doc, root_element);
	}

	xmlSaveFormatFileEnc(invesorsXmlPass, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
}

char * DataHandler_investorsToXmlStr(DataHandler_t self, char * mem_p){
    xmlDoc * doc = NULL;
	xmlNode * root_element = NULL;
	xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");
	root_element = xmlNewNode(NULL, "investors");
	xmlDocSetRootElement(doc, root_element);

	int investorsNum = List_getSize(self->investorsList);
	for(int i = 0; i < investorsNum; i++){
        Investor_toXmlNode(List_get(self->investorsList, i, NULL), doc, root_element);
	}

	xmlDocDumpMemory(doc, &xmlStr, NULL);

	strcpy(mem_p, xmlStr);

    free(xmlStr);
    xmlFreeDoc(doc);
    return mem_p;
}

int DataHandler_getInvestorsNum(DataHandler_t self){
    return List_getSize(self->investorsList);
}

Investor_t DataHandler_getInvestor(DataHandler_t self, unsigned int index){
    return List_get(self->investorsList, index, NULL);
}

void DataHandler_addInvestor(DataHandler_t self, Investor_t inv){
    List_append(self->investorsList, inv);
}

void DataHandler_deleteInvestor(DataHandler_t self, unsigned int index){
    Investor_delete(List_remove(self->investorsList, index, NULL));
}

void DataHandler_updateInvestor(DataHandler_t self, unsigned int index, const char * key, const char * value){
    Investor_update(List_get(self->investorsList, index, NULL), key, value);
}
