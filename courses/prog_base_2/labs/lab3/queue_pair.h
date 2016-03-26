#ifndef QUEUE_PAIR_H_INCLUDED
#define QUEUE_PAIR_H_INCLUDED

#include "queue.h"

#define QUEUE_PAIR_MAX_SIZE 10

typedef struct QueuePair * QueuePair_t;

typedef void (* QueuePair_empty_CB)(QueuePair_t sender);
typedef void (* QueuePair_singleOverflow_CB)(Queue_t queue);
typedef void (* QueuePair_fullOverflow_CB)(QueuePair_t sender);

QueuePair_t QueuePair_new(QueuePair_empty_CB cb1, QueuePair_singleOverflow_CB cb2, QueuePair_fullOverflow_CB cb3);
void QueuePair_enqueueFirst(QueuePair_t self, int val);
void QueuePair_enqueueSecond(QueuePair_t self, int val);
int QueuePair_dequeueFirst(QueuePair_t self);
int QueuePair_dequeueSecond(QueuePair_t self);
Queue_t QueuePair_getFirst(QueuePair_t self);
Queue_t QueuePair_getSecond(QueuePair_t self);
void QueuePair_delete(QueuePair_t self);

#endif // QUEUE_PAIR_H_INCLUDED
