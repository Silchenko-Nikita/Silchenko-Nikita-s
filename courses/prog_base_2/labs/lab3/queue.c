#include <stdlib.h>

#include "queue.h"
#include "list.h"

static struct Event{
    void * callback;
    void * data;
};

static struct Node{
    int val;
    struct Node * next;
};

typedef struct Node Node_s;
typedef struct Event Event_s;

static struct Queue{
     Node_s * head;
     Node_s * tail;
     unsigned int size;

     Queue_t boundQueue;

     List_t singleOverflowEvents;
     List_t pairEmptyEvents;
     List_t pairOverflowEvents;
};

Queue_t Queue_new(){
    Queue_t self = (Queue_t)malloc(sizeof(struct Queue));

    if(NULL == self){
        puts("Error allocating memory");
        exit(1);
    }

    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
    self->boundQueue = NULL;
    self->singleOverflowEvents = List_new();
    self->pairEmptyEvents = List_new();
    self->pairOverflowEvents = List_new();

    return self;
}

void Queue_delete(Queue_t self){
    if(self->boundQueue != NULL) self->boundQueue->boundQueue = NULL;

    Node_s * curNode = self->head;

    while(curNode != NULL){
        Node_s * nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }

    List_deepDelete(self->singleOverflowEvents, free);
    List_deepDelete(self->pairEmptyEvents, free);
    List_deepDelete(self->pairOverflowEvents, free);

    free(self);
}

void Queue_subscribeSingleOverflowEvent(Queue_t self, singleQueueEvent_cb singleOverflow_cb, void * data){
    Event_s * subscriber = ( Event_s *)malloc(sizeof(Event_s));

    if(NULL == subscriber){
        puts("Error allocating memory");
        exit(1);
    }

    subscriber->callback = singleOverflow_cb;
    subscriber->data = data;

    List_append(self->singleOverflowEvents, subscriber);
}

void Queue_subscribePairOverflowEvent(Queue_t self, queuePairEvent_cb pairOverflow_cb, void * data){
    Event_s * subscriber = ( Event_s *)malloc(sizeof(Event_s));

    if(NULL == subscriber){
        puts("Error allocating memory");
        exit(1);
    }

    subscriber->callback = pairOverflow_cb;
    subscriber->data = data;

    List_append(self->pairOverflowEvents, subscriber);
}

void Queue_subscribePairEmptyEvent(Queue_t self, queuePairEvent_cb pairEmpty_cb, void * data){
    Event_s * subscriber = ( Event_s *)malloc(sizeof(Event_s));

    if(NULL == subscriber){
        puts("Error allocating memory");
        exit(1);
    }

    subscriber->callback = pairEmpty_cb;
    subscriber->data = data;

    List_append(self->pairEmptyEvents, subscriber);
}

void Queue_bindQueues(Queue_t queue1, Queue_t queue2){

    queue1->boundQueue = queue2;
    queue2->boundQueue = queue1;
}

static void _sinleOverflow(Queue_t self){
    int listSize = List_getSize(self->singleOverflowEvents);
    for(int i = 0; i < listSize; i++){
        Event_s * event = List_get(self->singleOverflowEvents, i, NULL);
        singleQueueEvent_cb fn = event->callback;
        if(NULL != fn) fn(self, event->data);
    }
}

static void _pairOverflow(Queue_t self){
    int listSize = List_getSize(self->pairOverflowEvents);
    for(int i = 0; i < listSize; i++){
        Event_s * event = List_get(self->pairOverflowEvents, i, NULL);
        queuePairEvent_cb fn = event->callback;
        if(NULL != fn) fn(self, event->data, self->boundQueue);
    }
}

static void _pairEmpty(Queue_t self){
    int listSize = List_getSize(self->pairEmptyEvents);
    for(int i = 0; i < listSize; i++){
        Event_s * event = List_get(self->pairEmptyEvents, i, NULL);
        queuePairEvent_cb fn = event->callback;
        if(NULL != fn) fn(self, event->data, self->boundQueue);
    }
}


void Queue_enqueue(Queue_t self, int val){
    Node_s * newNode = (Node_s *)malloc(sizeof(Node_s));
    if(NULL == newNode){
        puts("Error allocating memory");
        exit(1);
    }

    newNode->next = NULL;
    newNode->val = val;
    if(Queue_isEmpty(self)){
        self->head = newNode;
    }else{
        self->tail->next = newNode;
    }
    self->tail = newNode;

    self->size++;

    if(self->size > 10){
        _sinleOverflow(self);

        if(NULL != self->boundQueue){
            if(Queue_getSize(self->boundQueue) < 9){
                Queue_enqueue(self->boundQueue, Queue_dequeue(self));
            }else{
                _pairOverflow(self);
            }
        }
    }
}

int Queue_dequeue(Queue_t self){
    if(Queue_isEmpty(self)) return 0;

    Node_s * toDelete = self->head;
    void * toReturn = toDelete->val;
    self->head = toDelete->next;
    if(self->head == NULL)
        self->tail = NULL;

    free(toDelete);

    self->size--;

    if(self->size == 0){
        if(NULL != self->boundQueue){
            if(Queue_getSize(self->boundQueue) > 1){
                Queue_enqueue(self, Queue_dequeue(self->boundQueue));
            }else{
                _pairEmpty(self);
            }
        }
    }
    return toReturn;
}

int Queue_head(Queue_t self){
    if(Queue_isEmpty(self)) return 0;

    return self->head->val;
}

bool Queue_isEmpty(Queue_t self){
    return self->head == NULL;
}

unsigned int Queue_getSize(Queue_t self){
    return self->size;
}
