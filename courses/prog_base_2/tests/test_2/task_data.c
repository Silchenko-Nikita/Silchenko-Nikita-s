#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "task_data.h"
#include "str_type.h"

TaskData_t TaskData_new(){
    return malloc(sizeof(struct TaskData));
}

void TaskData_delete(TaskData_t self){
    free(self);
}

void TaskData_getFromXmlStr(TaskData_t self, const char * xmlStr){
    xmlDocPtr doc = NULL;
    xmlNodePtr root_element = NULL;

    doc = xmlReadMemory(xmlStr, strlen(xmlStr), NULL, NULL, NULL);

    if ((root_element = xmlDocGetRootElement(doc)) == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
    }

    if(xmlStrcmp(root_element->name, (const xmlChar *)"object")){
        xmlFreeDoc(doc);
    }

    for(xmlNodePtr cur = root_element->xmlChildrenNode; cur != NULL; cur = cur->next){
        if (!xmlStrcmp(cur->name, (const xmlChar *)"author")) {
            xmlChar * author = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->author, (const char *) author);
            free(author);
        }else if (!xmlStrcmp(cur->name, (const xmlChar *)"book")) {
            xmlChar * book = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            strcpy(self->book, (const char *) book);
            free(book);
        }
    }

    xmlFreeDoc(doc);
}

char * TaskData_toXmlStr(TaskData_t self, const char * mem_p){
    xmlDoc * doc = NULL;
	xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");

	xmlNodePtr data = NULL;
    data = xmlNewNode(NULL, "object");

    xmlDocSetRootElement(doc, data);

    char buff[256];
    xmlNewChild(data, NULL, "author", self->author);
    xmlNewChild(data, NULL, "book", self->book);
    xmlNewChild(data, NULL, "time", ctime(&self->time));

	xmlDocDumpMemory(doc, &xmlStr, NULL);

    strcpy(mem_p, xmlStr);
    free(xmlStr);
    xmlFreeDoc(doc);
    return mem_p;
}
