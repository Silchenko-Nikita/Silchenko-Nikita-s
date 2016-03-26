#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "queue.h"

typedef bool (*check_f)(Queue_t queue);
typedef void (*react_f)(void);

typedef struct dynamic_s {
    HANDLE hLib;
    check_f check;
    react_f react;
} dynamic_t;

dynamic_t * dynamic_init(const char * dllName);
void dynamic_clean(dynamic_t * dyn);

const char * userChoice();

int main(void) {
    const int MAX_QUEUE_SIZE = 7;
    Queue_t queue = Queue_new();
    const char * dllName = userChoice();
    dynamic_t * dll = dynamic_init(dllName);
    if (NULL == dll) {
        printf("Can't load dynamic!\n");
        return 1;
    }
    if (NULL == dll->check) {
        printf("Can't get compare function!\n");
        return 1;
    }
    if (NULL == dll->react) {
        printf("Can't get reaction function!\n");
        return 1;
    }
    puts("Dynamic loaded!");
    srand(time(NULL));
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        int prec =  !(rand() % 3) ? 1 + rand() % 15 : 0;
        printf("%dth day's precipitation: %d\n", i, prec);
        Queue_enqueue(queue, prec);
    }
    if(dll->check(queue))
        dll->react();
    Queue_delete(queue);
    return 0;
}

const char * userChoice() {
    int dllNum = 0;
    while (dllNum < 1 || dllNum > 2) {
        printf("Choose DLL to load:\n1. lab2_dll1\n2. lab2_dll2\n> ");
        scanf("%i", &dllNum);
        if (dllNum == 1) {
            return "lab2_dll1.dll";
        } else if (dllNum == 2) {
            return "lab2_dll2.dll";
        }
    }
    return NULL;
}

dynamic_t * dynamic_init(const char * dllName) {
    dynamic_t * dyn = (dynamic_t *)malloc(sizeof(struct dynamic_s));
    dyn->hLib = LoadLibrary(dllName);
    dyn->check = NULL;
    dyn->react = NULL;
    if (NULL != dyn->hLib) {
        dyn->check = (check_f)GetProcAddress(dyn->hLib, "check");
        dyn->react = (react_f)GetProcAddress(dyn->hLib, "react");
        return dyn;
    } else {
        return NULL;
    }
}

void dynamic_clean(dynamic_t * dyn) {
    FreeLibrary(dyn->hLib);
    free(dyn);
}
