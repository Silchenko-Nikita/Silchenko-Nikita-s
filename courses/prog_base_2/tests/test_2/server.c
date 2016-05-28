#include <libxml/parser.h>
#include <libxml/tree.h>

#include "libsocket/socket.h"

#include "server.h"

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

static const char * const HttpRequestStrFormat =
        "%s HTTP/1.1\r\n"
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

    char root[128];
    getTok(uri, 0, "/", root);

    if (!strcmp(root, "info")) {
        _reply_Student(clientSock);
    } else if (!strcmp(root, "favicon.ico")){
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

/*
static void _reply_XML(Web_t self, HttpRequest_t httpRequest, socket_t * clientSock){
    const char * uri = HttpRequest_getURI(httpRequest);
    HTTP_REQUEST_METHOD method = HttpRequest_getMethod(httpRequest);

    char child1[128], child2[128];
    getTok(uri, 1, "/", child1);
    getTok(uri, 2, "/", child2);

    if (!strcmp(child1, "investors")) {
        if(!strcmp(child2, "")){
            if (method == HTTP_GET){
                char * charP = HttpRequest_getArgsVal(httpRequest, "gtinvestment");
                double minInvestment = !strcmp(charP, "[empty]") ? MAX_DOUBLE : atof(charP);

                printf("%d\n", minInvestment);

                charP = HttpRequest_getArgsVal(httpRequest, "ltinvestment");
                double maxInvestment = !strcmp(charP, "[empty]") ? MAX_DOUBLE : atof(charP);

                printf("%d\n", maxInvestment);

                charP = HttpRequest_getArgsVal(httpRequest, "gtprojects");
                int minProjectsNumber = !strcmp(charP, "[empty]") ? MIN_INT : atoi(charP);

                printf("%f\n", minProjectsNumber);

                charP = HttpRequest_getArgsVal(httpRequest, "ltprojects");
                int maxProjectsNumber = !strcmp(charP, "[empty]") ? MAX_INT : atoi(charP);

                printf("%f\n", maxProjectsNumber);

                _writeInvestors_XML(self, clientSock, minInvestment, maxInvestment, minProjectsNumber, maxProjectsNumber);
            }else if (method == HTTP_POST){
                _addInvestorIfPossible(self, clientSock, httpRequest);
            }else{
               _replyInvalidMethod(clientSock);
            }
        }else if (isNonNegatInteger(child2)){
            int index = atoi(child2);

            if(_checkIndex(self, index) == SERVER_INDEX_ERROR){
                _replyInvalidIndex(clientSock, index);
            }else{
                if (method == HTTP_GET){
                    _writeInvestor_XML(self, clientSock, index);
                } else if(method == HTTP_OPTIONS){
                    _optionsOkReply(clientSock);
                } else if (method == HTTP_DELETE){
                    DataHandler_deleteInvestor(self->dataHandler, index);
                    _successDeleteReply(clientSock, index);
                }else{
                    _replyInvalidMethod(clientSock);
                }
            }
        }else{
            _replyInvalidUri(clientSock);
        }
    }else{
        _replyInvalidUri(clientSock);
    }
}

static void _reply_HTML(Web_t self, HttpRequest_t httpRequest, socket_t * clientSock){
    const char * uri = HttpRequest_getURI(httpRequest);
    HTTP_REQUEST_METHOD method = HttpRequest_getMethod(httpRequest);

    char child0[128], child1[128];
    getTok(uri, 0, "/", child0);
    getTok(uri, 1, "/", child1);

    if (!strcmp(child0, "")) {
        if (method == HTTP_GET){
            _replyHomepage_HTML(self, clientSock);
        }else{
            _replyInvalidMethod(clientSock);
        }
    }else if (!strcmp(child0, "investors")){
        if(!strcmp(child1, "")){
            if (method == HTTP_GET){
                char * charP = HttpRequest_getArgsVal(httpRequest, "gtinvestment");
                double minInvestment = !strcmp(charP, "[empty]") ? MIN_DOUBLE : atof(charP);

                printf("%d\n", minInvestment);

                charP = HttpRequest_getArgsVal(httpRequest, "ltinvestment");
                double maxInvestment = !strcmp(charP, "[empty]") ? MIN_DOUBLE : atof(charP);

                printf("%d\n", maxInvestment);

                charP = HttpRequest_getArgsVal(httpRequest, "gtprojects");
                int minProjectsNumber = !strcmp(charP, "[empty]") ? MIN_INT : atoi(charP);

                printf("%f\n", minProjectsNumber);

                charP = HttpRequest_getArgsVal(httpRequest, "ltprojects");
                int maxProjectsNumber = !strcmp(charP, "[empty]") ? MAX_INT : atoi(charP);

                printf("%f\n", maxProjectsNumber);

                _writeInvestors_HTML(self, clientSock, minInvestment, maxInvestment, minProjectsNumber, maxProjectsNumber);
            }else if (method == HTTP_POST){
                _addInvestorIfPossible(self, clientSock, httpRequest);
            }else{
               _replyInvalidMethod(clientSock);
            }
        }else if(isNonNegatInteger(child1)){
            int index = atoi(child1);

            if(_checkIndex(self, index) == SERVER_INDEX_ERROR){
                _replyInvalidIndex(clientSock, index);
            }else{
                if (method == HTTP_GET){
                    _writeInvestor_HTML(self, clientSock, index);
                }else if(method == HTTP_OPTIONS){
                    _optionsOkReply(clientSock);
                }else if (method == HTTP_DELETE){
                    DataHandler_deleteInvestor(self->dataHandler, index);
                    _successDeleteReply(clientSock, index);
                }else{
                   _replyInvalidMethod(clientSock);
                }
            }
        }else{
            _replyInvalidUri(clientSock);
        }
    }else if (!strcmp(child0, "new-investor")){
        if (method == HTTP_GET){
            _replyNewInvestor_HTML(self, clientSock);
        }else{
            _replyInvalidMethod(clientSock);
        }
    }else{
        _replyInvalidUri(clientSock);
    }
}*/

