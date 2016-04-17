#include <stdio.h>
#include <strings.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "server.h"

#include "str_type.h"
#include "list.h"
#include "investor.h"

// server functionality

static const char * const httpSuccessReplyStrFormat = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n%s";

static const char * const httpInvalidUriReplyStrFormat = "HTTP/1.1 400 INVALID_URI\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n%s";

static const char * const httpInvalidKeyReplyStrFormat = "HTTP/1.1 401 INVALID_KEY\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n%s";

static const char * const httpInvalidValueReplyStrFormat = "HTTP/1.1 402 INVALID_VALUE\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n%s";

static const char * const httpMethtodValueReplyStrFormat = "HTTP/1.1 403 INVALID_METHOD\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n%s";

static const char * const invesorsXmlPass = "data/investors.xml";
static List_t investors; // think of this global var as of fields of a singleton

SERVER_ERROR Server_init(){
    investors = List_new();

    xmlDocPtr doc = NULL;
    xmlNodePtr root_element = NULL;

    LIBXML_TEST_VERSION    // Macro to check API for match with
                             // the DLL we are using

    if ((doc = xmlParseFile(invesorsXmlPass)) == NULL){
        fprintf(stderr, "error: could not parse file %s\n", invesorsXmlPass);
        return SERVER_FILE_ERROR;
    }

    if ((root_element = xmlDocGetRootElement(doc)) == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return SERVER_FILE_ERROR;
    }

    if(xmlStrcmp(root_element->name, (const xmlChar *)"investors")){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return SERVER_FILE_ERROR;
    }

    for(xmlNodePtr cur = root_element->xmlChildrenNode; cur != NULL; cur = cur->next){
        if (!xmlStrcmp(cur->name, (const xmlChar *)"investor")) {
            List_append(investors, Investor_new(doc, cur));
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return SERVER_OK;
}

void Server_writeInvestors(socket_t * client){
    char investorsStr[10000];
    investorsStr[0] = '\0';
    int investorsNum = List_getSize(investors);
    for(int i = 0; i < investorsNum; i++){
        char investorStr[512];
        char buff[520];
        sprintf(buff, "%d) %s\r\n\r\n", i + 1, Investor_toString(List_get(investors, i, NULL), investorStr));
        strcat(investorsStr, buff);
    }
    char res[10000];
    sprintf(res, httpSuccessReplyStrFormat, strlen(investorsStr), investorsStr);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_emptySuccessReply(socket_t * client){
    char res[64];
    sprintf(res, httpSuccessReplyStrFormat, 0, "");

    socket_write_string(client, res);
    socket_close(client);
}

void Server_successDeleteReply(socket_t * client, unsigned int index){
    char buff[48];
    char res[128];
    sprintf(buff, "Investor %zu was successfully deleted", index);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_successUpdateReply(socket_t * client, unsigned int index){
    char buff[48];
    char res[128];
    sprintf(buff, "Investor %zu was successfully updated", index);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_replyInvalidUri(socket_t * client){
    char buff[16];
    char res[80];
    strcpy(buff, "INVALID URI");
    sprintf(res, httpInvalidUriReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_replyInvalidMethod(socket_t * client){
    char buff[16];
    char res[80];
    strcpy(buff, "INVALID METHOD");
    sprintf(res, httpMethtodValueReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_replyInvalidIndex(socket_t * client, unsigned int index){
    char buff[24];
    char res[80];
    sprintf(buff, "INVALID INDEX: %zu", index);
    sprintf(res, httpInvalidUriReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_replyInvalidKey(socket_t * client, const char * key){
    char buff[256];
    char res[320];
    sprintf(buff, "INVALID KEY: %s", key);
    sprintf(res, httpInvalidKeyReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

void Server_replyInvalidValue(socket_t * client, const char * value){
    char buff[256];
    char res[320];
    sprintf(buff, "INVALID VALUE: %s", value);
    sprintf(res, httpInvalidValueReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);
}

SERVER_ERROR Server_writeInvestor(socket_t * client, unsigned int index){
    if(index < 1 || index > List_getSize(investors)) return SERVER_INDEX_ERROR;

    int investorsNum = List_getSize(investors);
    char investorStr[512];
    char buff[520];
    sprintf(buff, "%d) %s\r\n\r\n", index, Investor_toString(List_get(investors, index - 1, NULL), investorStr));
    char res[520];
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff), buff);

    socket_write_string(client, res);
    socket_close(client);

    return SERVER_OK;
}

SERVER_ERROR Server_deleteInvestor(unsigned int index){
    if(index < 1 || index > List_getSize(investors)) return SERVER_INDEX_ERROR;

    List_remove(investors, index - 1, NULL);

    return SERVER_OK;
}

SERVER_ERROR Server_checkIndex(unsigned int index){
    if(index < 1 || index > List_getSize(investors)) return SERVER_INDEX_ERROR;
    else return SERVER_OK;
}

SERVER_ERROR Server_checkKeyValue(const char * key, const char * value){
    int keyLen = strlen(key);
    int valLen = strlen(value);
    if(valLen == 0 || valLen > 23 || keyLen == 0 || keyLen > 23){
        return SERVER_VALUE_ERROR;
    }

    if(!strcmp(key, "name") || !strcmp(key, "surname") || !strcmp(key, "corporation_country")){
        if(!isAlpha(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "corporation_name")){
        if(!isAlphaNum(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "birthdate")){
        if(!isDate(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "year")){
        if(!isNonNegatInteger(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "investment")){
        if(!isNonNegatReal(value)){
            return SERVER_VALUE_ERROR;
        }
    }else{
        return SERVER_KEY_ERROR;
    }
    return SERVER_OK;
}

SERVER_ERROR Server_updateInvestor(unsigned int index, const char * key, const char * value){
    if(index < 1 || index > List_getSize(investors)) return SERVER_INDEX_ERROR;

    Investor_update(List_get(investors, index - 1, NULL), key, value);

    return SERVER_OK;
}


void Server_serializeData(){
    xmlDoc * doc = NULL;
	xmlNode * root_element = NULL;
	xmlNode * investorNode = NULL;

	doc = xmlNewDoc("1.0");
	root_element = xmlNewNode(NULL, "investors");
	xmlDocSetRootElement(doc, root_element);

	int investorsNum = List_getSize(investors);
	for(int i = 0; i < investorsNum; i++){
        investorNode = xmlNewChild(root_element, NULL, "investor", NULL);
        Investor_serialize(List_get(investors, i, NULL), doc, investorNode);
	}

	xmlSaveFormatFileEnc(invesorsXmlPass, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}


void Server_finalize(socket_t * client){
    List_deepDelete(investors, Investor_delete);
}
