#include <stdio.h>
#include <libxml/tree.h>

#include "investor.h"

int main(int argc, char **argv){
    const char * const filepass = "data/investors.xml";
    unsigned int MAX_INVESTORS_NUM = 8;
    Investor_t investors[MAX_INVESTORS_NUM];
    memset(investors, 0, sizeof(Investor_t)*MAX_INVESTORS_NUM);

    xmlDocPtr doc = NULL;
    xmlNodePtr root_element = NULL;

    LIBXML_TEST_VERSION    // Macro to check API for match with
                             // the DLL we are using

    if ((doc = xmlParseFile(filepass)) == NULL){
        fprintf(stderr, "error: could not parse file %s\n", filepass);
        return 1;
    }

    if ((root_element = xmlDocGetRootElement(doc)) == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return 1;
    }

    if(xmlStrcmp(root_element->name, (const xmlChar *)"investors")){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return 0;
    }

    int investorsNum = 0;
    for(xmlNodePtr cur = root_element->xmlChildrenNode; cur != NULL && investorsNum < MAX_INVESTORS_NUM; cur = cur->next){
        if (!xmlStrcmp(cur->name, (const xmlChar *)"investor")) {
            investors[investorsNum] = Investor_new(doc, cur);
            investorsNum++;
        }
    }

    for(int i = 0; i < investorsNum; i++){
        Investor_print(investors[i]);
    }

    for(int i = 0; i < investorsNum; i++){
        Investor_delete(investors[i]);
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}
