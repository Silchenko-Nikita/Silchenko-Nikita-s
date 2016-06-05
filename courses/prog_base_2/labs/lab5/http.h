#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

typedef struct HttpRequest HttpRequest_s;
typedef HttpRequest_s * HttpRequest_t;

typedef enum HTTP_REQUEST_METHOD{
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE
} HTTP_REQUEST_METHOD;

HttpRequest_t HttpRequest_new(); // res is not initialized
void HttpRequest_delete(HttpRequest_t self);

void HttpRequest_parse(HttpRequest_t self, const char * request); // for init
HTTP_REQUEST_METHOD HttpRequest_getMethod(HttpRequest_t self);
const char * HttpRequest_getURI(HttpRequest_t self);

const char * HttpRequest_getFormVal(HttpRequest_t self, const char * key);
const char * HttpRequest_getFormKey(HttpRequest_t self, unsigned int index);

const char * HttpRequest_getArgsVal(HttpRequest_t self, const char * key);
const char * HttpRequest_getArgsKey(HttpRequest_t self, unsigned int index);

const char * HttpRequest_getFormLen(HttpRequest_t self);
const char * HttpRequest_getArgsLen(HttpRequest_t self);

#endif // HTTP_H_INCLUDED
