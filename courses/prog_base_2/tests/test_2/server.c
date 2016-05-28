#include <libxml/parser.h>
#include <libxml/tree.h>

#include <time.h>

#include "libsocket/socket.h"

#include "server.h"
#include "task_data.h"

#include "http.h"

static struct Server{
    socket_t * serverSock;
};

static enum SERVER_ERROR{
    SERVER_OK,
    SERVER_FILE_ERROR,
    SERVER_INDEX_ERROR,
    SERVER_KEY_ERROR,
    SERVER_VALUE_ERROR,
    SERVER_ARGS_ERROR
};

typedef enum SERVER_ERROR SERVER_ERROR;

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

static const char * const htmlDocStrFormat =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta charset=\"ASCI\">"
        "%s"
        "</head>"
        "<body>"
        "%s"
        "</body>"
        "</html>";


static void _emptySuccessReply(socket_t * clientSock);
static void _reply(Server_t self, HttpRequest_t httpRequest, socket_t * clientSock);
static void _reply_Student(socket_t * clientSock);

Server_t Server_new(int port){
    Server_t self = (Server_t) malloc(sizeof(Server_s));
    self->serverSock = socket_new();
    socket_bind(self->serverSock, port);
    socket_listen(self->serverSock);

    return self;
}

void Server_delete(Server_t self){
    free(self);
}

void Server_listen(Server_t self){
    char input[8000];

    HttpRequest_t httpRequest = HttpRequest_new();
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

    char child1[128];
    getTok(uri, 0, "/", child1);

    if (!strcmp(child1, "info")) {
        _reply_Student(clientSock);
    } if (!strcmp(child1, "external")) {
        char buff[256];
        _getDataStr(buff);
        puts(buff);
        TaskData_t td = TaskData_new();
        TaskData_getFromXmlStr(td, buff);
        time(&td->time);

        TaskData_toXmlStr(td, buff);
        char buff1[512];
        sprintf(buff1, httpReplyStrFormat, "200 OK", "text/xml", strlen(buff), buff);

        TaskData_delete(td);
        socket_write_string(clientSock, buff);
        socket_close(clientSock);
    } else if (!strcmp(child1, "favicon.ico")){
        _emptySuccessReply(clientSock);
    }
}

static void _emptySuccessReply(socket_t * clientSock){
    char res[64];
    sprintf(res, httpSuccessReplyStrFormat, 0, "");

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _reply_Student(socket_t * clientSock){
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

void _getDataStr(char * mem_p){
    socket_t* clientSock = socket_new ();
    socket_connect(clientSock, "216.58.209.49", 80);

    char uri [256];

    strcpy (uri, "http://pb-homework.appspot.com/test/var/34?format=xml");

    char req [1024];

    sprintf (req, httpRequestStrFormat, "GET", uri, NULL, NULL, NULL);

    socket_write(clientSock, req, strlen (req));
    char responce [1024];
    socket_read (clientSock, responce, 1024);

    int contentLength = 0;
    sscanf (strstr(responce, "Content-Length: ") + strlen ("Content-Length: "), "%d", &contentLength);
    strcpy(mem_p, strstr (responce, "\r\n\r\n") + 4);

    socket_free(clientSock);
    return mem_p;
}
