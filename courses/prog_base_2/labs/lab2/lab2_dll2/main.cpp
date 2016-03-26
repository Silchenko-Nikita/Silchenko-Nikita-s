#include <stdio.h>
#include <stdbool.h>

#include "main.h"

bool DLL_EXPORT check(Queue_t queue){
    if(Queue_getSize(queue) < 3) return false;

    while(Queue_getSize(queue) > 3){
        Queue_dequeue(queue);
    }

    while(!Queue_isEmpty(queue)){
        if(Queue_dequeue(queue) > 0) return false;
    }
    return true;
}

void DLL_EXPORT react(){
    puts("Still sunny");
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
