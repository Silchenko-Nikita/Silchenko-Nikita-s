#include <libxml/parser.h>
#include <libxml/tree.h>

#include <time.h>

#include "libsocket/socket.h"

#include "server.h"

#include "db_manager.h"
#include "task_data.h"

#include "http.h"

// code isn't beautiful but i don't care

static struct Server{
    socket_t * serverSock;
    DB_t * db;
};


static enum FILE_DELETE_STATUS{
    FILE_OK,
    FILE_NOT_EXISTS,
    FILE_ERROR
};

typedef enum SERVER_ERROR SERVER_ERROR;
typedef enum FILE_DELETE_STATUS FILE_DELETE_STATUS;

static const char * const httpReplyStrFormat =
        "HTTP/1.1 %sr\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpSuccessReplyStrFormat =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpRequestStrFormat =
        "%s %s HTTP/1.1\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static char * _serializeInvestors(List_t invs, char * mem_p);
static void _emptySuccessReply(socket_t * clientSock);
static void _reply(Server_t self, HttpRequest_t httpRequest, socket_t * clientSock);
static void _replyStudent(socket_t * clientSock);
static const char * _getFileStatusStr(FILE_DELETE_STATUS status);
static void _replyFileDeleteStatus(socket_t * clientSock, FILE_DELETE_STATUS status);

Server_t Server_new(int port){
    Server_t self = (Server_t) malloc(sizeof(Server_s));
    self->serverSock = socket_new();
    self->db = DB_new("data/investors.db");
    socket_bind(self->serverSock, port);

    return self;
}

void Server_delete(Server_t self){
    socket_free(self->serverSock);
    socket_free(self->db);
    free(self);
}

void Server_listen(Server_t self){
    char input[8000];

    HttpRequest_t httpRequest = HttpRequest_new();
    socket_listen(self->serverSock);
    while(1) {
        socket_t * clientSock = socket_accept(self->serverSock);
        socket_read(clientSock, input, sizeof(input));

        HttpRequest_parse(httpRequest, input);

        _reply(self, httpRequest, clientSock);

        socket_free(clientSock);
    }
    HttpRequest_delete(httpRequest);
}

static void _reply(Server_t self, HttpRequest_t httpRequest, socket_t * clientSock){
    const char * uri = HttpRequest_getURI(httpRequest);
    char * uriBuff[256];
    strcpy(uriBuff, uri);
    char child1[128], child2[128];
    getTok(uriBuff, 0, "/", child1);

    if (strcmp("", getTok(uriBuff, 1, "/", child2)) && strcmp("file", child1)) {
        _emptySuccessReply(clientSock);
    }

    if(HttpRequest_getMethod(httpRequest) == HTTP_GET){

        if (!strcmp(child1, "info")) {
            _replyStudent(clientSock);
        }else if (!strcmp(child1, "external")) {
            char buff[100000];
            _getDataStr(buff);
            TaskData_t td = TaskData_new();
            TaskData_getFromXmlStr(td, buff);
            time(&td->time);

            TaskData_toXmlStr(td, buff);
            char buff1[100000];
            sprintf(buff1, httpReplyStrFormat, "200 OK", "text/xml", strlen(buff), buff);

            TaskData_delete(td);
            socket_write_string(clientSock, buff);
            socket_close(clientSock);
        } else if (!strcmp(child1, "database")){
            char buff[10000];
            List_t invs = List_new();
            DB_getAllInvestors(self->db, invs);
            _serializeInvestors(invs, buff);

            char buff1[512];
            sprintf(buff1, httpReplyStrFormat, "200 OK", "text/xml", strlen(buff), buff);
            socket_write_string(clientSock, buff);
            socket_close(clientSock);

            List_delete(invs);
        }else if(!strcmp(child1, "file") && !strcmp(HttpRequest_getArgsVal(httpRequest, "action"), "delete")){
            char filePath[256];
            strcpy(filePath, strstr(strstr(uri, "/") + 1, "/") + 1);

            FILE_DELETE_STATUS rc;
            if(file_exists(filePath)){
                if(file_remove(filePath)){
                    rc = FILE_OK;
                }else{
                    rc = FILE_ERROR;
                }
            }else{
                rc = FILE_NOT_EXISTS;
            }
            _replyFileDeleteStatus(clientSock, rc);
        } else{
            _emptySuccessReply(clientSock);
        }
    }else{
        _emptySuccessReply(clientSock);
    }
}

static void _emptySuccessReply(socket_t * clientSock){
    char res[64];
    sprintf(res, httpSuccessReplyStrFormat, 0, "");

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyStudent(socket_t * clientSock){
    xmlDoc * doc = NULL;
	xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");

	xmlNodePtr student = NULL;
    student = xmlNewNode(NULL, "student");

    xmlDocSetRootElement(doc, student);

    xmlNewChild(student, NULL, "name", "Nikita Silchenko");
    xmlNewChild(student, NULL, "group", "KP-52");
    xmlNewChild(student, NULL, "variant", "34");

	xmlDocDumpMemory(doc, &xmlStr, NULL);


    char buff[512];
    sprintf(buff, httpReplyStrFormat, "200 OK", "text/xml", strlen(xmlStr), xmlStr);

    socket_write_string(clientSock, buff);
    socket_close(clientSock);

	free(xmlStr);
    xmlFreeDoc(doc);
}

char * _serializeInvestors(List_t invs, char * mem_p){
    xmlDoc * doc = NULL;
	xmlNode * root_element = NULL;
    xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");
	root_element = xmlNewNode(NULL, "investors");
	xmlDocSetRootElement(doc, root_element);

	int investorsNum = List_getSize(invs);
	for(int i = 0; i < investorsNum; i++){
        Investor_toXmlNode(List_get(invs, i, NULL), doc, root_element);
	}

	xmlSaveFormatFileEnc("data/investors.xml", doc, "UTF-8", 1);

	xmlDocDumpMemory(doc, &xmlStr, NULL);

    strcpy(mem_p, xmlStr);
    free(xmlStr);
    xmlFreeDoc(doc);
    return mem_p;
}

static const char * _getFileStatusStr(FILE_DELETE_STATUS status){
    static const char * const strs[] = {
        "deleted",
        "not exists",
        "fail"
    };
    return strs[status];
}

void _replyFileDeleteStatus(socket_t * clientSock, FILE_DELETE_STATUS status){
    xmlDoc * doc = NULL;
	xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");

	xmlNodePtr statusNode = NULL;
    statusNode = xmlNewNode(NULL, "status");
    xmlDocSetRootElement(doc, statusNode);

    xmlNewChild(statusNode, NULL, "code", _getFileStatusStr(status));
	xmlDocDumpMemory(doc, &xmlStr, NULL);

    char buff[512];
    sprintf(buff, httpReplyStrFormat, "200 OK", "text/xml", strlen(xmlStr), xmlStr);

    socket_write_string(clientSock, buff);
    socket_close(clientSock);

	free(xmlStr);
    xmlFreeDoc(doc);
}

void _getDataStr(char * mem_p){
    socket_t* clientSock = socket_new ();
    socket_connect(clientSock, "216.58.209.49", 80);

    char uri[256];

    strcpy(uri, "http://pb-homework.appspot.com/test/var/34?format=xml");

    char req[1024];

    sprintf (req, httpRequestStrFormat, "GET", uri, NULL, NULL, NULL);

    socket_write(clientSock, req, strlen (req));
    char responce [1024];
    socket_read (clientSock, responce, 1024);

    strcpy(mem_p, strstr (responce, "\r\n\r\n") + 4);

    socket_free(clientSock);
    return mem_p;
}
