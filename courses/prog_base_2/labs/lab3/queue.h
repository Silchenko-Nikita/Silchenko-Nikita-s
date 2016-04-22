#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include<stdbool.h>

typedef struct Queue * Queue_t;

typedef void (* singleQueueEvent_cb)(Queue_t caller, void * callerData);
typedef void (* queuePairEvent_cb)(Queue_t caller, void * callerData, Queue_t boundQueue);

Queue_t Queue_new();
void Queue_delete(Queue_t self);

void Queue_bindQueues(Queue_t queue1, Queue_t queue2);

void Queue_subscribeSingleOverflowEvent(Queue_t self, singleQueueEvent_cb singleOverflow_cb, void * data);
void Queue_subscribePairOverflowEvent(Queue_t self, queuePairEvent_cb pairOverflow_cb, void * data);
void Queue_subscribePairEmptyEvent(Queue_t self, queuePairEvent_cb pairEmpty_cb, void * data);

void Queue_enqueue(Queue_t self, int val);
int Queue_dequeue(Queue_t self);
int Queue_head(Queue_t self);
bool Queue_isEmpty(Queue_t self);
unsigned int Queue_getSize(Queue_t self);


#endif // QUEUE_H_INCLUDED
