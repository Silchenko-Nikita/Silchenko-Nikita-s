#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "libsocket/socket.h"

// server functionality

typedef enum SERVER_ERROR{
    SERVER_OK,
    SERVER_FILE_ERROR,
    SERVER_INDEX_ERROR,
    SERVER_KEY_ERROR,
    SERVER_VALUE_ERROR
} SERVER_ERROR;

SERVER_ERROR Server_init();
void Server_finalize();

void Server_emptySuccessReply(socket_t * client);
void Server_successDeleteReply(socket_t * client, unsigned int index);
void Server_successUpdateReply(socket_t * client, unsigned int index);
void Server_replyInvalidUri(socket_t * client);
void Server_replyInvalidMethod(socket_t * client);
void Server_replyInvalidIndex(socket_t * client, unsigned int index);
void Server_replyInvalidKey(socket_t * client, const char * key);
void Server_replyInvalidValue(socket_t * client, const char * value);

SERVER_ERROR Server_checkIndex(unsigned int index);
SERVER_ERROR Server_checkKeyValue(const char * key, const char * value);

void Server_writeInvestors(socket_t * client);
SERVER_ERROR Server_writeInvestor(socket_t * client, unsigned int index);

SERVER_ERROR Server_deleteInvestor(unsigned int index);

void Server_serializeData();

#endif // SERVER_H_INCLUDED
