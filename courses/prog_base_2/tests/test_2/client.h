#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

char * Client_requestHTTP(const char * method, const char * path, const char * msg, const char * ipaddress, int portNumber, char * mem_p);

#endif // CLIENT_H_INCLUDED
