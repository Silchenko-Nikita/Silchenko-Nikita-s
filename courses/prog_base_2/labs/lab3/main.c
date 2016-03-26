#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "queue_pair.h"

int randInt(){
    return rand() - RAND_MAX/2;
}

bool randBool(){
    return rand() % 2;
}

void empty(QueuePair_t sender){
    puts("empty");
}

void singleOverflow(Queue_t queue){
    puts("singleOverflow");
}

void fullOverflow(QueuePair_t sender){
    puts("fullOverflow");
}

int main()
{
    singleOverflow(NULL);
//    moduleTests_QueuePair();
//    QueuePair_t queuePair = QueuePair_new(empty, singleOverflow, fullOverflow);
//    for(int i = 0; i < 50; i++){
//        int rndInt = randInt();
//        if(randBool()){
//            if(rndInt >= 0){
//                QueuePair_enqueueFirst(queuePair, rndInt);
//            }else{
//                QueuePair_dequeueFirst(queuePair);
//            }
//        }else{
//            if(rndInt >= 0){
//                QueuePair_enqueueSecond(queuePair, rndInt);
//            }else{
//                QueuePair_dequeueSecond(queuePair);
//            }
//        }
//    }
//
//    Queue_delete(queuePair);
    return 0;
}
