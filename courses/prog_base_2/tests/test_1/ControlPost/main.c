#include <stdlib.h>

#include "ControlPost.h"

#include "list.h"

typedef struct Event * Event_t;

static struct Event{
    void * cb;
    void * receiver;
    void * data;
};

static struct Post{

    List_t * events;
};

static struct Transist{

    List_t * events;
};

static struct Car{

    List_t * events;
};

Post_t Post_new(){
    Post_t self = (Post_t) malloc(sizeof(struct Post));
    return self;
}

Post_delete(Post_t self){
    List_deepDelete(self->events, free);
    free(self);
}

void Post_subscribe/*Event*/(Post_t self, void * receiver, PostEvent_CB cb, void * data){
    Event_t event = (Event_t)malloc(sizeof(struct Event));
    event->receiver = receiver;
    event->data = data;
    List_append(self->events, event);
}

void * Post_event(Post_t self){
    int lSize = List_getSize(self->events);
    for(int i = 0; i < lSize; i++){
        Event_t ev = List_get(self->events, i, NULL);
        if(ev->cb != NULL){
            PostEvent_CB fn = ev->cb;
            fn(ev->receiver, self, ev->data);
        }
    }
}

//

Transist_t Transist_new(){
    Transist_t self = (Transist_t) malloc(sizeof(struct Transist));
    return self;
}

Transist_delete(Transist_t self){
    List_deepDelete(self->events, free);
    free(self);
}

void Transist_subscribe/*Event*/(Transist_t self, void * receiver, TransistEvent_CB cb, void * data){
    Event_t event = (Event_t)malloc(sizeof(struct Event));
    event->receiver = receiver;
    event->data = data;
    List_append(self->events, event);
}

//

Car_t Car_new(){
    Car_t self = (Car_t) malloc(sizeof(struct Car));
    return self;
}

Car_delete(Car_t self){
    List_deepDelete(self->events, free);
    free(self);
}

void Car_subscribe/*Event*/(Car_t self, void * receiver, CarEvent_CB cb, void * data){
    Event_t event = (Event_t)malloc(sizeof(struct Event));
    event->receiver = receiver;
    event->data = data;
    List_append(self->events, event);
}
