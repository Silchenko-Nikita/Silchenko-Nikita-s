#include "libsocket/socket.h"

#include "client.h"

static const char * const httpRequestStrFormat =
        "%s %s HTTP/1.1\r\n"
        "Content-Type: text\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s";


char * Client_requestHTTP(const char * method, const char * path, const char * msg, const char * ipaddress, int portNumber, char * mem_p){
    socket_t * clientSock = socket_new();
    socket_connect(clientSock, ipaddress, portNumber);

    char pathBuff[256];
    sprintf(pathBuff, "%s:%d%s", ipaddress, portNumber, path);

    char req[256];
    sprintf(req, httpRequestStrFormat, method, pathBuff, strlen(msg), msg);

    socket_write_string(clientSock, req);
    socket_read(clientSock, mem_p, 100000);
    return * mem_p;
}
