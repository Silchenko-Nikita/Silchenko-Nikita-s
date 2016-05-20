#ifndef CONTROLPOST_H_INCLUDED
#define CONTROLPOST_H_INCLUDED

typedef struct Post * Post_t;
typedef struct Transist * Transist_t;
typedef struct Car * Car_t;

typedef void (* PostEvent_CB)(void * receiver, Post_t sender, void * data);
typedef void (* TransistEvent_CB)(void * receiver, Transist_t sender, void * data);
typedef void (* CarEvent_CB)(void * receiver, Car_t sender, void * data);

typedef enum POST_ERROR{
    POST_OK
} POST_ERROR;

typedef enum TRANSIST_ERROR{
    TRANSIST_OK
} TRANSIST_ERROR;

typedef enum CAR_ERROR{
    CAR_OK
} CAR_ERROR;

//

Post_t Post_new();
Post_delete(Post_t self);

//

Transist_t Transist_new();
Transist_delete(Transist_t self);

//

Car_t Car_new();
Car_delete(Car_t self);

#endif // CONTROLPOST_H_INCLUDED
