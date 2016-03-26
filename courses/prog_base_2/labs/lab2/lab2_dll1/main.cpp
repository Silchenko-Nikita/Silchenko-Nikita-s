#include <stdio.h>
#include <stdbool.h>

#include "main.h"

bool DLL_EXPORT check(Queue_t queue){
    while(Queue_getSize(queue) > 7){
        Queue_dequeue(queue);
    }

    int sum = 0;
    while(!Queue_isEmpty(queue)){
        sum += Queue_dequeue(queue);
    }

    if(sum > 20) return true;
    else return false;
}

void DLL_EXPORT react(){
    puts("Rainy");
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
