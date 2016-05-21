#include <stdlib.h>
#include <stdio.h>

#include "ControlPost.h"

#include "list.h"

typedef struct Event * Event_t;

static struct Event{
    void * cb;
    void * receiver;
    void * data;
};

static struct Post{
    int xCoord, yCoord; // abstract map coords
    List_t transists;
    List_t wantedCars; // ids

    List_t wantedCarEvents;
};

static struct Transist{
    Car_t car;
    TRANSIST_DEST dest;
};

static struct Car{
    char id[64];
    Person_t owner;
};

static struct Person{
    char name[64];
    char surname[64];
};

Post_t Post_new(int xCoord, int yCoord){
    Post_t self = (Post_t) malloc(sizeof(struct Post));
    self->wantedCars = List_new();
    self->wantedCarEvents = List_new();
    self->transists = List_new();
    self->xCoord = xCoord;
    self->yCoord = yCoord;
    return self;
}

void Post_addWantedCar(Post_t self, char * id){
    List_append(self->wantedCars, id);
}

void Post_pass(Post_t self, Transist_t transist){
    if(Post_carIsWanted(self, transist->car->id)){
        int lSize = List_getSize(self->wantedCarEvents);

        for(int i = 0; i < lSize; i++){
            Event_t ev = List_get(self->wantedCarEvents, i, NULL);
            if(ev->cb != NULL){
                PostEvent_CB fn = ev->cb;
                fn(ev->receiver, self, ev->data);
                break;
            }
        }
    }
    List_append(self->transists, transist);
}

bool Post_carIsWanted(Post_t self, const char * carId){
    int lSize = List_getSize(self->wantedCars);
    for(int i = 0; i < lSize; i++){
        if(!strcmp(List_get(self->wantedCars, i, NULL), carId)){
            return true;
        }
    }
    return false;
}

void Post_clearTransistsHistory(Post_t self){
   while(List_getSize(self->transists) > 0){
        List_remove(self->transists, 0, NULL);
   }
}

unsigned int Post_getTransistsNum(Post_t self){
    return List_getSize(self->transists);
}

void Post_getCoords(Post_t self, int * xCoord_p, int * yCoord_p){
    if (xCoord_p != NULL) *xCoord_p = self->xCoord;
    if (yCoord_p != NULL) *yCoord_p = self->yCoord;
}

List_t Post_getTransists(Post_t self){
    int lSize = List_getSize(self->transists);
    List_t retList = List_new();
    for(int i = 0; i < lSize; i++){
        List_append(retList, List_get(self->transists, i, NULL));
    }
    return retList;
}

List_t Post_getWantedCars(Post_t self){
    int lSize = List_getSize(self->wantedCars);
    List_t retList = List_new();
    for(int i = 0; i < lSize; i++){
        List_append(retList, List_get(self->wantedCars, i, NULL));
    }
    return retList;
}

List_t Post_getLastNCars(Post_t self, unsigned int N){
    int lSize = List_getSize(self->transists);
    if (N >= lSize) printf("Post: invalid index %d", N);
    List_t retList = List_new();
    for(int i = lSize - N; i < lSize; i++){
        List_append(retList, Transist_getCar(List_get(self->transists, i, NULL)));
    }
    return retList;
}

List_t Post_getCars(Post_t self, TRANSIST_DEST dest){
    int lSize = List_getSize(self->transists);
    List_t retList = List_new();
    for(int i = 0; i < lSize; i++){
        Transist_t transist = List_get(self->transists, i, NULL);
        if(dest == transist->dest){
            List_append(retList, Transist_getCar(transist));
        }
    }
    return retList;
}

Post_delete(Post_t self){
    List_deepDelete(self->wantedCarEvents, free);
    List_deepDelete(self->transists, Transist_delete);
    List_delete(self->wantedCars);
    free(self);
}

void Post_subscribeWantedCarTransist(Post_t self, void * receiver, PostEvent_CB cb, void * data){
    Event_t event = (Event_t)malloc(sizeof(struct Event));
    event->receiver = receiver;
    event->cb = cb;
    event->data = data;
    List_append(self->wantedCarEvents, event);
}

void Post_unsubscribeWantedCarTransist(Post_t self, void * receiver, PostEvent_CB cb){
    int lSize = List_getSize(self->wantedCarEvents);
    for(int i = 0; i < lSize; i++){
        Event_t ev = List_get(self->wantedCarEvents, i, NULL);
        if(ev->receiver == receiver && ev->cb == cb){
            List_remove(self->wantedCarEvents, i, NULL);
        }
    }
}

//

Transist_t Transist_new(Car_t car, TRANSIST_DEST dest){
    Transist_t self = (Transist_t) malloc(sizeof(struct Transist));
    self->car = car;
    self->dest = dest;
    return self;
}

Car_t Transist_getCar(Transist_t self){
    Car_t car = Car_new(car->id);
    *car = *self->car;
    return car;
}

Transist_delete(Transist_t self){
    Car_delete(self->car);
    free(self);
}

//

Car_t Car_new(const char * id){
    Car_t self = (Car_t) malloc(sizeof(struct Car));
    strcpy(self->id, id);
    self->owner = NULL;
    return self;
}

void Car_setOwner(Car_t self, Person_t owner){
    self->owner = owner;
}

Person_t Car_getOwner(Car_t self){
    Person_t owner = Person_new(self->owner->name, self->owner->surname);
    *owner = *self->owner;
    return owner;
}

const char * Car_getID(Car_t self){
    return self->id;
}

Car_delete(Car_t self){
    free(self);
}

//

Person_t Person_new(char * name, char * surname){
    Person_t self = (Person_t) malloc(sizeof(struct Person));
    strcpy(self->name, name);
    strcpy(self->surname, surname);
    return self;
}

char * Person_toString(Person_t self, char * mem_p){
    sprintf(mem_p, "name: %s, surname: %s", self->name, self->surname);
    return mem_p;
}

void Person_delete(Person_t self){
    free(self);
}
