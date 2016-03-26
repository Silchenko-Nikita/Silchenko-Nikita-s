#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue_pair.h"

static struct QueuePair {
    Queue_t q1, q2;

    QueuePair_empty_CB empty_f;
    QueuePair_singleOverflow_CB singleOverflow_f;
    QueuePair_fullOverflow_CB fullOverflow_f;
};

QueuePair_t QueuePair_new(QueuePair_empty_CB cb1, QueuePair_singleOverflow_CB cb2, QueuePair_fullOverflow_CB cb3){
    QueuePair_t self = (QueuePair_t)malloc(sizeof(struct QueuePair));
    if(NULL == self){
        puts("Error allocating memory");
        exit(1);
    }

    self->empty_f = cb1;
    self->singleOverflow_f = cb2;
    self->fullOverflow_f = cb3;

    self->q1 = Queue_new();
    self->q2 = Queue_new();

    for(int i = 0; i < 3; i++){
        Queue_enqueue(self->q1, randInt());
        Queue_enqueue(self->q2, randInt());
    }

    return self;
}

void QueuePair_enqueueFirst(QueuePair_t self, int val){
    Queue_enqueue(self->q1, val);

    if(Queue_getSize(self->q1) > QUEUE_PAIR_MAX_SIZE){
        if(Queue_getSize(self->q2) < 9){
            Queue_enqueue(self->q2, Queue_dequeue(self->q1));
            if(self->singleOverflow_f != NULL) self->singleOverflow_f(self->q1);
        }
        else
            if(self->fullOverflow_f != NULL) self->fullOverflow_f(self);
    }
}

void QueuePair_enqueueSecond(QueuePair_t self, int val){
    Queue_enqueue(self->q2, val);


    if(Queue_getSize(self->q2) > QUEUE_PAIR_MAX_SIZE){
        if(Queue_getSize(self->q1) < 9){
            Queue_enqueue(self->q1, Queue_dequeue(self->q2));
            if(self->singleOverflow_f != NULL) self->singleOverflow_f(self->q2);
        }
        else
            if(self->fullOverflow_f != NULL) self->fullOverflow_f(self);
    }
}

int QueuePair_dequeueFirst(QueuePair_t self){
    int v = Queue_dequeue(self->q1);

    if(Queue_isEmpty(self->q1)){
        if(Queue_getSize(self->q2) >= 2)
            Queue_enqueue(self->q1, Queue_dequeue(self->q2));
        else
            if(self->empty_f != NULL) self->empty_f(self);
    }

    return v;
}

int QueuePair_dequeueSecond(QueuePair_t self){
    int v = Queue_dequeue(self->q2);

    if(Queue_isEmpty(self->q2)){
        if(Queue_getSize(self->q1) >= 2)
            Queue_enqueue(self->q2, Queue_dequeue(self->q1));
        else
            if(self->empty_f != NULL) self->empty_f(self);
    }

    return v;
}

Queue_t QueuePair_getFirst(QueuePair_t self){
    return self->q1;
}

Queue_t QueuePair_getSecond(QueuePair_t self){
    return self->q2;
}

void QueuePair_delete(QueuePair_t self){
    Queue_delete(self->q1);
    Queue_delete(self->q2);
    free(self);
}
