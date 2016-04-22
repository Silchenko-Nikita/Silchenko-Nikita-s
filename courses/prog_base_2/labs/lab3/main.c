#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "queue.h"

int randInt(){
    return rand() - RAND_MAX/2;
}

int randBool(){
    return rand() % 2;
}


void singleQueueOverflow1(Queue_t caller, void * callerData){
    puts("singleQueueOverflow1");
    puts(callerData);
    puts("\n");
}

void singleQueueOverflow2(Queue_t caller, void * callerData){
    puts("singleQueueOverflow2");
    puts(callerData);
    puts("\n");
}

void queuePairOverflow(Queue_t caller, void * callerData, Queue_t boundQueue){
    puts("queuePairOverflow");
    puts(callerData);
    puts("\n");
}

void queuePairEmpty(Queue_t caller, void * callerData, Queue_t boundQueue){
    puts("queuePairEmpty");
    puts(callerData);
    puts("\n");
}

int main()
{
//    moduleTests_Queue();
    Queue_t queue1 = Queue_new();
    Queue_t queue2 = Queue_new();

    for(int i = 0; i < 10; i++){
        Queue_enqueue(queue1, randInt());
        Queue_enqueue(queue2, randInt());
    }

    Queue_bindQueues(queue1, queue2);

    Queue_subscribeSingleOverflowEvent(queue1, singleQueueOverflow1, "queue1");
    Queue_subscribeSingleOverflowEvent(queue1, singleQueueOverflow2, "queue1");

    Queue_subscribeSingleOverflowEvent(queue2, singleQueueOverflow1, "queue2");
    Queue_subscribeSingleOverflowEvent(queue2, singleQueueOverflow2, "queue2");

    Queue_subscribePairOverflowEvent(queue1, queuePairOverflow, "queue1");
    Queue_subscribePairEmptyEvent(queue1, queuePairEmpty, "queue1");

    Queue_subscribePairOverflowEvent(queue2, queuePairOverflow, "queue2");
    Queue_subscribePairEmptyEvent(queue2, queuePairEmpty, "queue2");

    for(int i = 0; i < 50; i++){
        int rndInt = randInt();
        if(randBool()){
            if(rndInt >= 0){
                Queue_enqueue(queue1, rndInt);
            }else{
                Queue_dequeue(queue1);
            }
        }else{
            if(rndInt >= 0){
                Queue_enqueue(queue2, rndInt);
            }else{
                Queue_dequeue(queue2);
            }
        }
    }

    Queue_delete(queue1);
    Queue_delete(queue2);
    return 0;
}
