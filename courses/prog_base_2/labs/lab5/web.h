#ifndef WEB_H_INCLUDED
#define WEB_H_INCLUDED

typedef struct Web Web_s;
typedef Web_s * Web_t;

Web_t Web_new(int port);
void Web_delete(Web_t self);

void Web_listen(Web_t self);

#endif // WEB_H_INCLUDED
