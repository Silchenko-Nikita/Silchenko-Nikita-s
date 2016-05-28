#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

typedef struct Server Server_s;
typedef Server_s * Server_t;

Server_t Server_new(int port);
void Server_delete(Server_t self);

void Server_listen(Server_t self);

#endif // SERVER_H_INCLUDED
