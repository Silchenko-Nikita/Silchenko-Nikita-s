#include <stdlib.h>
#include <string.h>

#include "http.h"

#define URI_MAX_LEN 256
#define KEY_MAX_LEN 256
#define VALUE_MAX_LEN 256
#define FORM_MAX_LEN 20

static struct HttpRequest {
    HTTP_REQUEST_METHOD method;
    char uri[URI_MAX_LEN];

    struct KeyVal{
        char key[KEY_MAX_LEN];
        char value[VALUE_MAX_LEN]
    } form[FORM_MAX_LEN];
    size_t formLen;
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
    return (HttpRequest_t) malloc(sizeof(HttpRequest_s));
}

void HttpRequest_delete(HttpRequest_t self){
    free(self);
}

void HttpRequest_parse(HttpRequest_t self, const char * request){
    size_t methodLen = 0;
    methodLen = strstr(request, " ") - request;
    if(methodLen > 24 || methodLen < 0) return;
    char methodStr[256] = "";
    memcpy(methodStr, request, methodLen);
    methodStr[methodLen] = '\0';
    self->method = getMethodByStr(methodStr);

    const char * uriStartPtr = request + methodLen + 1;
    size_t uriLen = strstr(uriStartPtr, " ") - uriStartPtr;
    memcpy(self->uri, uriStartPtr, uriLen);
    self->uri[uriLen] = '\0';

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

const char * HttpRequest_getArg(HttpRequest_t self, const char * key){
    for (int i = 0; i < self->formLen; i++){
        if(!strcmp(self->form[i].key, key)){
            return self->form[i].value;
        }
    }
}

const char * HttpRequest_getKey(HttpRequest_t self, unsigned int index){
    if(index >= FORM_MAX_LEN) return "";
    else return self->form[index].key;
}


const char * HttpRequest_getArgsNum(HttpRequest_t self){
    return self->formLen;
}

