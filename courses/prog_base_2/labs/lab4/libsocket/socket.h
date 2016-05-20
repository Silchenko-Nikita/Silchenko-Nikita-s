#ifndef socket_H_INCLUDED
#define socket_H_INCLUDED

#define CONNECT_OK 0

typedef struct socket_s socket_t;

typedef enum socket_status_s {
    SOCKET_ERR = -1
} socket_status_t;

void lib_init(void);
void lib_free(void);

socket_t * socket_new(void);
void socket_free(socket_t * self);

const char * socket_getIPAddress(socket_t * self);
int socket_getPortNumber(socket_t * self);

int socket_bind(socket_t * socket, int portNumber);
int socket_listen(socket_t * socket);
socket_t * socket_accept(socket_t * socket);
int socket_connect(socket_t * sock, const char * ipaddress, int portNumber);
int socket_read(socket_t * sock, char * recvBuff, int recvSize);
int socket_write(socket_t * conn, const char * msg, int msgSize);
int socket_write_string(socket_t * conn, const char * msg);
void socket_close(socket_t * conn);

#endif // socket_H_INCLUDED
