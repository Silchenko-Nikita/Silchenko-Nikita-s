#ifndef CONTROLPOST_H_INCLUDED
#define CONTROLPOST_H_INCLUDED

typedef struct Post * Post_t;
typedef struct Transist * Transist_t;
typedef struct Car * Car_t;

Post_t Post_new();
Post_delete(Post_t self);

//

Transist_t Transist_new();
Transist (Transist_t self);

//

Car_t Car_new();
Car_delete(Car_t self);

#endif // CONTROLPOST_H_INCLUDED
