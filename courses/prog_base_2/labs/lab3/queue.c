#include <stdlib.h>

#include "queue.h"

static struct Node{
    int val;
    void ** next;
};

typedef struct Node Node_s;

static struct Queue{
     Node_s * head;
     Node_s * tail;
     unsigned int size;
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

    return self;
}

void Queue_delete(Queue_t self){
    Node_s * curNode = self->head;

    while(curNode != NULL){
        Node_s * nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }

    free(self);
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
