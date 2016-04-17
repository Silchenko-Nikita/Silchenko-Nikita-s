#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libsocket/socket.h"

#include "http.h"
#include "server.h"
#include "str_type.h"

#define PORT 5000

int main() {
    lib_init();
    Server_init();
    socket_t * server = socket_new();
    socket_bind(server, PORT);
    socket_listen(server);

    char buf[10000];
    socket_t * client = NULL;
    HttpRequest_t httpRequest = HttpRequest_new();
    while(1) {
        client = socket_accept(server);
        socket_read(client, buf, sizeof(buf));

        HttpRequest_parse(httpRequest, buf);
        const char * uri = HttpRequest_getURI(httpRequest);

        HTTP_REQUEST_METHOD method = HttpRequest_getMethod(httpRequest);

        char rootName[48];
        getTok(uri, 0, "/", rootName);

        if (!strcmp(rootName, "investors")) {
            char child[24];
            getTok(uri, 1, "/", child);

            if (!strcmp(child, "")) {
                if (method == HTTP_GET){
                    Server_writeInvestors(client);
                } else {
                    Server_replyInvalidMethod(client);
                }
            } else if (isNonNegatInteger(child)){
                int index = atoi(child);

                if(Server_checkIndex(index) == SERVER_INDEX_ERROR){
                    Server_replyInvalidIndex(client, index);
                }else{
                    if (method == HTTP_GET){
                        Server_writeInvestor(client, index);
                    } else if (method == HTTP_DELETE){
                        Server_deleteInvestor(index);
                        Server_serializeData();
                        Server_successDeleteReply(client, index);
                    } else if (method == HTTP_POST){
                        int argsValid = 1;
                        int argsNum = HttpRequest_getArgsNum(httpRequest);
                        for(int i = 0; i < argsNum; i++){
                            const char * key = HttpRequest_getKey(httpRequest, i);
                            const char * val = HttpRequest_getArg(httpRequest, key);
                            SERVER_ERROR err = Server_checkKeyValue(key, val);
                            if(err == SERVER_KEY_ERROR){
                                Server_replyInvalidKey(client, key);
                                argsValid = 0;
                                break;
                            } else if (err == SERVER_VALUE_ERROR){
                                Server_replyInvalidValue(client, val);
                                argsValid = 0;
                                break;
                            }
                        }

                        if(argsValid){
                            for(int i = 0; i < argsNum; i++){
                                const char * key = HttpRequest_getKey(httpRequest, i);
                                const char * val = HttpRequest_getArg(httpRequest, key);
                                SERVER_ERROR err = Server_checkKeyValue(key, val);
                                Server_updateInvestor(index, key, val);
                            }
                            Server_serializeData();
                            Server_successUpdateReply(client, index);
                        }
                    }else{
                        Server_replyInvalidMethod(client);
                    }
                }
            }else{
                Server_replyInvalidUri(client);
            }
        } else if (!strcmp(rootName, "favicon.ico")){
            Server_emptySuccessReply(client);
        } else{
            Server_replyInvalidUri(client);
        }
    }
    HttpRequest_delete(httpRequest);
    Server_finalize();
    socket_free(server);
    lib_free();
    return 0;
}
