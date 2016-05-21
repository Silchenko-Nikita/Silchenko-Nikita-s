#ifndef CONTROLPOST_H_INCLUDED
#define CONTROLPOST_H_INCLUDED

#include <stdbool.h>

#include "list.h"

typedef struct Post * Post_t;
typedef struct Transist * Transist_t;
typedef struct Car * Car_t;
typedef struct Person * Person_t;

typedef void (* PostEvent_CB)(void * receiver, Post_t sender, void * data);

typedef enum TRANSIST_DEST{
    TRANSIST_DEST_FROM,
    TRANSIST_DEST_TO
} TRANSIST_DEST;

//

Post_t Post_new(int xCoord, int yCoord);
void Post_addWantedCar(Post_t self, char * id);
void Post_pass(Post_t self, Transist_t transist);
bool Post_carIsWanted(Post_t self, const char * carId);
void Post_clearTransistsHistory(Post_t self);
unsigned int Post_getTransistsNum(Post_t self);
void Post_getCoords(Post_t self, int * xCoord_p, int * yCoord_p); // abstract map coords

List_t Post_getTransists(Post_t self);
List_t Post_getWantedCars(Post_t self);
List_t Post_getLastNCars(Post_t self, unsigned int N);
List_t Post_getCars(Post_t self, TRANSIST_DEST dest);

Post_delete(Post_t self);

void Post_subscribeWantedCarTransist(Post_t self, void * receiver, PostEvent_CB cb, void * data);
void Post_unsubscribeWantedCarTransist(Post_t self, void * receiver, PostEvent_CB cb);

//

Transist_t Transist_new(Car_t car, TRANSIST_DEST dest);
Car_t Transist_getCar(Transist_t self);
Transist_delete(Transist_t self);
//

Car_t Car_new(const char * id);
void Car_setOwner(Car_t self, Person_t owner);
Person_t Car_getOwner(Car_t self);
const char * Car_getID(Car_t self);
Car_delete(Car_t self);

//

Person_t Person_new(char * name, char * surname);
char * Person_toString(Person_t self, char * mem_p);
void Person_delete(Person_t self);


#endif // CONTROLPOST_H_INCLUDED
