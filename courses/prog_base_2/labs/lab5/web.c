#include "libsocket/socket.h"

#include "web.h"

#include "http.h"
#include "str_type.h"
#include "data_handler.h"

#define MAX_INT 65536
#define MIN_INT -65535
#define MAX_DOUBLE 99999.0
#define MIN_DOUBLE -99999.0

static struct Web{
    DataHandler_t dataHandler;
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

static const char * const httpSuccessReplyStrFormat =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpInvalidUriReplyStrFormat =
        "HTTP/1.1 404 INVALID_URI\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpInvalidKeyReplyStrFormat =
        "HTTP/1.1 401 INVALID_KEY\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpInvalidValueReplyStrFormat =
        "HTTP/1.1 402 INVALID_VALUE\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpMethtodValueReplyStrFormat =
        "HTTP/1.1 400 INVALID_METHOD\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";

static const char * const httpOptionsOkStr =
            "HTTP/1.1 200 OK\n"
            "Access-Control-Allow-Origin: *\n"
            "Access-Control-Allow-Methods: DELETE\n"
            "\n";

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


static void _reply(Web_t self, HttpRequest_t httpRequest, socket_t * clientSock);
static void _writeInvestors_XML(Web_t self, socket_t * clientSock,  double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber);
static void _writeInvestor_XML(Web_t self, socket_t * clientSock, unsigned int index);
static void _emptySuccessReply(socket_t * clientSock);
static void _successDeleteReply(socket_t * clientSock, unsigned int index);
static void _successUpdateReply(socket_t * clientSock, unsigned int index);
static void _optionsOkReply(socket_t * clientSock);
static void _successAdditionReply(socket_t * clientSock);
static void _replyInvalidUri(socket_t * clientSock);
static void _replyInvalidMethod(socket_t * clientSock);
static void _replyInvalidIndex(socket_t * clientSock, unsigned int index);
static void _replyInvalidKey(socket_t * clientSock, const char * key);
static void _replyInvalidValue(socket_t * clientSock, const char * value);
static void _replyInvalidArgsNum(socket_t * clientSock);
static void _replyHomepage_HTML(Web_t self, socket_t * clientSock);
static void _writeInvestors_HTML(Web_t self, socket_t * clientSock,  double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber);
static void _writeInvestor_HTML(Web_t self, socket_t * clientSock, unsigned int index);
static void _replyNewInvestor_HTML(Web_t self, socket_t * clientSock);
void _validateArgsVals(double * minInvestment, double * maxInvestment, int * minProjectsNumber, int * maxProjectsNumber);

static void _addInvestorIfPossible(Web_t self, socket_t * clientSock, HttpRequest_t httpRequest);

static SERVER_ERROR _checkArgs(HttpRequest_t httpRequest, socket_t * clientSock);
static SERVER_ERROR _checkIndex(Web_t self, int index);
static SERVER_ERROR _checkKeyValue(const char * key, const char * value);

Web_t Web_new(int port){
    Web_t self = (Web_t) malloc(sizeof(Web_s));
    self->dataHandler = DataHandler_new("data/investors.db");
    self->serverSock = socket_new();
    socket_bind(self->serverSock, port);
    socket_listen(self->serverSock);

    return self;
}

void Web_delete(Web_t self){
    free(self);
}

void Web_listen(Web_t self){
    char input[8000];

    HttpRequest_t httpRequest = HttpRequest_new();
    while(1) {
        socket_t * clientSock = socket_accept(self->serverSock);
        socket_read(clientSock, input, sizeof(input));

        HttpRequest_parse(httpRequest, input);

        _reply(self, httpRequest, clientSock);

        socket_free(clientSock);
    }
}

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
                double minInvestment = !strcmp(charP, "[empty]") ? MIN_DOUBLE : atof(charP);

                charP = HttpRequest_getArgsVal(httpRequest, "ltinvestment");
                double maxInvestment = !strcmp(charP, "[empty]") ? MAX_DOUBLE : atof(charP);

                charP = HttpRequest_getArgsVal(httpRequest, "gtprojects");
                int minProjectsNumber = !strcmp(charP, "[empty]") ? MIN_INT : atoi(charP);

                charP = HttpRequest_getArgsVal(httpRequest, "ltprojects");
                int maxProjectsNumber = !strcmp(charP, "[empty]") ? MAX_INT : atoi(charP);

                _validateArgsVals(&minInvestment, &maxInvestment, &minProjectsNumber, &maxProjectsNumber);
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

                charP = HttpRequest_getArgsVal(httpRequest, "ltinvestment");
                double maxInvestment = !strcmp(charP, "[empty]") ? MAX_DOUBLE : atof(charP);

                charP = HttpRequest_getArgsVal(httpRequest, "gtprojects");
                int minProjectsNumber = !strcmp(charP, "[empty]") ? MIN_INT : atoi(charP);

                charP = HttpRequest_getArgsVal(httpRequest, "ltprojects");
                int maxProjectsNumber = !strcmp(charP, "[empty]") ? MAX_INT : atoi(charP);

                _validateArgsVals(&minInvestment, &maxInvestment, &minProjectsNumber, &maxProjectsNumber);
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
}

static void _reply(Web_t self, HttpRequest_t httpRequest, socket_t * clientSock){
    const char * uri = HttpRequest_getURI(httpRequest);

    char root[128];
    getTok(uri, 0, "/", root);

    if (!strcmp(root, "api")) {
        _reply_XML(self, httpRequest, clientSock);
    } else if (!strcmp(root, "favicon.ico")){
        _emptySuccessReply(clientSock);
    }else{
        _reply_HTML(self, httpRequest, clientSock);
    }
}

static void _replyHomepage_HTML(Web_t self, socket_t * clientSock){
    char buff[512];
    char buff1[512];
    char res[512];
    strcpy(buff, "<h2>Hello, visitor!</h2><br>"
           "<a href=\"/investors\">investors</a>");
    sprintf(buff1, htmlDocStrFormat, "", buff);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff1), buff1);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _writeInvestors_HTML(Web_t self, socket_t * clientSock,  double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber){
    char buff[10000] = "";
    char buff1[10000] = "";
    char res[10000] = "";

    strcpy(buff, "<a><b>Investors:</b></a><br><ol type=\"1\" start=\"0\">");

    int invsNum = DataHandler_getInvestorsNum(self->dataHandler);

    void _foreachInv_appendInv_HTML(Investor_t inv){
        char buff2[256];
        sprintf(buff2, "<li><a href=\"/investors/%d\">%s %s<a></li>", inv->id, inv->name, inv->surname);
        strcat(buff, buff2);
    }

    DataHandler_foreachInv(self->dataHandler, _foreachInv_appendInv_HTML, minInvestment, maxInvestment, minProjectsNumber, maxProjectsNumber);

    strcat(buff, "</ol><a href=\"/new-investor\">new</a>");

    sprintf(buff1, htmlDocStrFormat, "", buff);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff1), buff1);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

void _validateArgsVals(double * minInvestment, double * maxInvestment, int * minProjectsNumber, int * maxProjectsNumber){
    int updateProjectsNumber = 0, updateInvestment = 0;

    updateProjectsNumber = ((*minInvestment > MIN_DOUBLE) || (*maxInvestment < MAX_DOUBLE));
    updateInvestment = ((*minProjectsNumber > MIN_INT) || (*maxProjectsNumber < MAX_INT));

    if(updateProjectsNumber){
        if(*minProjectsNumber == MIN_INT && *maxProjectsNumber == MAX_INT){
            *minProjectsNumber == MAX_INT;
            *maxProjectsNumber = MIN_INT;
        }
    }

    if(updateInvestment){
        if(*minInvestment == MIN_DOUBLE && *maxInvestment == MAX_DOUBLE){
            *minInvestment == MAX_DOUBLE;
            *maxInvestment = MIN_DOUBLE;
        }
    }
}

static void _writeInvestor_HTML(Web_t self, socket_t * clientSock, unsigned int index){
    char buff[1000] = "";
    char buff1[1000] = "";
    char res[1000] = "";

    Investor_t inv = DataHandler_getInvestor(self->dataHandler, index);
    Investor_toString(inv, buff1);
    sprintf(buff, "<a>%s</a><br><a href=\"#\" onclick=\"doDelete()\">Delete</a>"
            "<script>"
            "function doDelete(){"
                "var xhttp = new XMLHttpRequest();"
                "xhttp.open(\"DELETE\", \"http://127.0.0.1:5000/investors/%d\", true);"
                "xhttp.send();"
            "}"
            "</script>", buff1, index);

    sprintf(buff1, htmlDocStrFormat, "", buff);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff1), buff1);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyNewInvestor_HTML(Web_t self, socket_t * clientSock){
    char buff[2000] = "";
    char buff1[2000] = "";
    char res[2000] = "";

    strcpy(buff, "<form action=\"/investors\" method=\"POST\">"
            "<fieldset>"
                "<legend>Investor</legend>"
                "name:<br>"
                "<input type=\"text\" name=\"name\"><br>"
                "surname:<br>"
                "<input type=\"text\" name=\"surname\"><br>"
                "birthdate:<br>"
                "<input type=\"text\" name=\"birthdate\"><br>"
                "investment:<br>"
                "<input type=\"text\" name=\"investment\"><br>"
                "projects number:<br>"
                "<input type=\"number\" name=\"projectsNumber\" min=\"0\"><br>"
                "<br><pre>Contacts:\n"
                "  telephone number:\n"
                "  <input type=\"text\" name=\"telephoneNumber\">\n"
                "  address\n"
                "  <input type=\"text\" name=\"address\">\n"
                "  email:\n"
                "  <input type=\"text\" name=\"email\"></pre>"
                "<input type=\"submit\" value=\"submit\"><br>"
            "</fieldset>"
            "</form>");

    sprintf(buff1, htmlDocStrFormat, "", buff);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff1), buff1);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static SERVER_ERROR _checkIndex(Web_t self, int index){
    if(DataHandler_getInvestor(self->dataHandler, index) == NULL) return SERVER_INDEX_ERROR;
    else return SERVER_OK;
}

static void _emptySuccessReply(socket_t * clientSock){
    char res[64];
    sprintf(res, httpSuccessReplyStrFormat, 0, "");

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _successDeleteReply(socket_t * clientSock, unsigned int index){
    char buff[64];
    char res[128];
    sprintf(buff, "Investor %zu was successfully deleted", index);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _successAdditionReply(socket_t * clientSock){
    char buff[64];
    char res[128];
    strcpy(buff, "Investor was successfully added");
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _successUpdateReply(socket_t * clientSock, unsigned int index){
    char buff[64];
    char res[128];
    sprintf(buff, "Investor %zu was successfully updated", index);
    sprintf(res, httpSuccessReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyInvalidUri(socket_t * clientSock){
    char buff[16];
    char res[80];
    strcpy(buff, "INVALID URI");
    sprintf(res, httpInvalidUriReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyInvalidMethod(socket_t * clientSock){
    char buff[16];
    char res[80];
    strcpy(buff, "INVALID METHOD");
    sprintf(res, httpMethtodValueReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyInvalidIndex(socket_t * clientSock, unsigned int index){
    char buff[24];
    char res[80];
    sprintf(buff, "INVALID INDEX: %zu", index);
    sprintf(res, httpInvalidUriReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyInvalidKey(socket_t * clientSock, const char * key){
    char buff[256];
    char res[320];
    sprintf(buff, "INVALID KEY: %s", key);
    sprintf(res, httpInvalidKeyReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _replyInvalidValue(socket_t * clientSock, const char * value){
    char buff[256];
    char res[320];
    sprintf(buff, "INVALID VALUE: %s", value);
    sprintf(res, httpInvalidValueReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _optionsOkReply(socket_t * clientSock){
    socket_write_string(clientSock, httpOptionsOkStr);
    socket_close(clientSock);
}

static void _replyInvalidArgsNum(socket_t * clientSock){
    char buff[64];
    char res[128];
    strcpy(buff, "NOT ALL FIELDS");
    sprintf(res, httpInvalidValueReplyStrFormat, strlen(buff), buff);

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _writeInvestors_XML(Web_t self, socket_t * clientSock,  double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber){
    char investorsStr[10000];
    investorsStr[0] = '\0';
    char res[10000];

    sprintf(res, httpSuccessReplyStrFormat, strlen(investorsStr), DataHandler_investorsToXmlStr(self->dataHandler, minInvestment, maxInvestment, minProjectsNumber, maxProjectsNumber, investorsStr));

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _writeInvestor_XML(Web_t self, socket_t * clientSock, unsigned int index){
    char investorsStr[1000];
    investorsStr[0] = '\0';
    char res[1000];
    sprintf(res, httpSuccessReplyStrFormat, strlen(investorsStr), Investor_toXmlStr(DataHandler_getInvestor(self->dataHandler, index), investorsStr));

    socket_write_string(clientSock, res);
    socket_close(clientSock);
}

static void _addInvestorIfPossible(Web_t self, socket_t * clientSock, HttpRequest_t httpRequest){
    int argsNum = HttpRequest_getFormLen(httpRequest);
            if(argsNum < 5){
                _replyInvalidArgsNum(clientSock);
            }else if(_checkArgs(httpRequest, clientSock) == SERVER_OK){
                Investor_t inv = Investor_new();
                for(int i = 0; i < argsNum; i++){
                    const char * key = HttpRequest_getFormKey(httpRequest, i);
                    const char * val = HttpRequest_getFormVal(httpRequest, key);

                    Investor_update(inv, key, val);

                }
                DataHandler_addInvestor(self->dataHandler, inv);
                _successAdditionReply(clientSock);

                Investor_delete(inv);
            }
}

static SERVER_ERROR _checkArgs(HttpRequest_t httpRequest, socket_t * clientSock){
    int argsNum = HttpRequest_getFormLen(httpRequest);
    for(int i = 0; i < argsNum; i++){
        const char * key = HttpRequest_getFormKey(httpRequest, i);
        const char * val = HttpRequest_getFormVal(httpRequest, key);
        SERVER_ERROR err = _checkKeyValue(key, val);
        if(err == SERVER_KEY_ERROR){
            _replyInvalidKey(clientSock, key);
            return SERVER_ARGS_ERROR;
        } else if (err == SERVER_VALUE_ERROR){
            _replyInvalidValue(clientSock, val);
            return SERVER_ARGS_ERROR;
        }
    }
    return SERVER_OK;
}


static SERVER_ERROR _checkKeyValue(const char * key, const char * value){
    int keyLen = strlen(key);
    int valLen = strlen(value);
    if(valLen == 0 || valLen > 23 || keyLen == 0 || keyLen > 23){
        return SERVER_VALUE_ERROR;
    }

    if(!strcmp(key, "name") || !strcmp(key, "surname")){
        if(!isAlpha(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "birthdate")){
        if(!isDate(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "projectsNumber")){
        if(!isNonNegatInteger(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "investment")){
        if(!isNonNegatReal(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "investment")){
        if(!isNonNegatReal(value)){
            return SERVER_VALUE_ERROR;
        }
    }else if(!strcmp(key, "telephoneNumber") || !strcmp(key, "address") || !strcmp(key, "email")){
        return SERVER_OK;
    }else{
        return SERVER_KEY_ERROR;
    }
    return SERVER_OK;
}
