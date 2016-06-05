#include <stdlib.h>
#include <string.h>

#include "http.h"
#include "str_type.h"

#define URI_MAX_LEN 256
#define KEY_MAX_LEN 256
#define VALUE_MAX_LEN 256
#define FORM_MAX_LEN 20

static struct KeyVal{
    char key[KEY_MAX_LEN];
    char value[VALUE_MAX_LEN]
};

static struct HttpRequest {
    HTTP_REQUEST_METHOD method;
    char uri[URI_MAX_LEN];

    struct KeyVal form[FORM_MAX_LEN];

    struct KeyVal args[FORM_MAX_LEN];

    size_t formLen;
    size_t argsLen;
};

static HTTP_REQUEST_METHOD getMethodByStr(const char * methodStr){
    static const char * repr[] = {
        "GET",
        "HEAD",
        "POST",
        "PUT",
        "DELETE",
        "CONNECT",
        "OPTIONS",
        "TRACE"
    };

    for(int i = 0; i < 8; i++){
        if(!strcmp(methodStr, repr[i])) return (HTTP_REQUEST_METHOD) i;
    }
}

HttpRequest_t HttpRequest_new(){
    HttpRequest_t self = (HttpRequest_t) malloc(sizeof(HttpRequest_s));
    self->formLen = 0;
    self->argsLen = 0;
    return self;
}

void HttpRequest_delete(HttpRequest_t self){
    free(self);
}

void HttpRequest_parse(HttpRequest_t self, const char * request){
    self->formLen = 0;
    self->argsLen = 0;
    size_t methodLen = 0;
    methodLen = strstr(request, " ") - request;
    if(methodLen > 24 || methodLen < 0) return;
    char methodStr[1000] = "";
    memcpy(methodStr, request, methodLen);
    methodStr[methodLen] = '\0';
    self->method = getMethodByStr(methodStr);

    const char * uriStartPtr = request + methodLen + 1;
    size_t uriLen = strstr(uriStartPtr, " ") - uriStartPtr;
    memcpy(self->uri, uriStartPtr, uriLen);
    self->uri[uriLen] = '\0';

    char buff[1000];
    if (getTok(self->uri, 1, "?", buff) != NULL){
        for(char * tok = strtok(buff, "&"); tok != NULL; tok = strtok(NULL, "&")){
            size_t keyLen = strstr(tok, "=") - tok;
            memcpy(self->args[self->argsLen].key, tok, keyLen);
            self->args[self->argsLen].key[keyLen] = '\0';

            strcpy(self->args[self->argsLen].value, tok + keyLen + 1);

            self->argsLen++;
        }
    }

    char * p = strstr(self->uri, "?");
    if(p != NULL) p[0] = '\0';

    if(self->method == HTTP_POST){
        const char * formStartPtr = NULL;
        formStartPtr = strstr(uriStartPtr, "\r\n\r\n");
        formStartPtr += 4;
        if(4 == formStartPtr){
            formStartPtr = NULL;
            formStartPtr = strstr(uriStartPtr, "\n\n");
            formStartPtr += 2;
        }

        char formBuff[FORM_MAX_LEN *(KEY_MAX_LEN + VALUE_MAX_LEN)];
        strcpy(formBuff, formStartPtr);

        self->formLen = 0;
        for(char * tok = strtok(formBuff, "&"); tok != NULL; tok = strtok(NULL, "&")){
            size_t keyLen = strstr(tok, "=") - tok;
            memcpy(self->form[self->formLen].key, tok, keyLen);
            self->form[self->formLen].key[keyLen] = '\0';

            strcpy(self->form[self->formLen].value, tok + keyLen + 1);

            self->formLen++;
        }
    }
}

HTTP_REQUEST_METHOD HttpRequest_getMethod(HttpRequest_t self){
    return self->method;
}

const char * HttpRequest_getURI(HttpRequest_t self){
    return self->uri;
}

const char * HttpRequest_getFormVal(HttpRequest_t self, const char * key){
    for (int i = 0; i < self->formLen; i++){
        if(!strcmp(self->form[i].key, key)){
            return self->form[i].value;
        }
    }
    static const char * empty = "[empty]";
    return empty;
}

const char * HttpRequest_getFormKey(HttpRequest_t self, unsigned int index){
    if(index >= FORM_MAX_LEN) return "";
    else return self->form[index].key;
}

const char * HttpRequest_getArgsVal(HttpRequest_t self, const char * key){
    for (int i = 0; i < self->argsLen; i++){
        if(!strcmp(self->args[i].key, key)){
            return self->args[i].value;
        }
    }
    static const char * empty = "[empty]";
    return empty;
}

const char * HttpRequest_getArgsKey(HttpRequest_t self, unsigned int index){
    if(index >= FORM_MAX_LEN) return "";
    else return self->args[index].key;
}


const char * HttpRequest_getFormLen(HttpRequest_t self){
    return self->formLen;
}

const char * HttpRequest_getArgsLen(HttpRequest_t self){
    return self->argsLen;
}

