#include "libsocket/socket.h"

#include "client.h"

void Client_request(const char * msg, const char * ipaddress, int portNumber){
    socket_t * clientSock = socket_new();
    socket_connect(clientSock, ipaddress, portNumber);
}
