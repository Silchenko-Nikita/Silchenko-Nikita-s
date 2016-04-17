#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>

typedef struct List * List_t;

typedef enum {
    LIST_OK,
    LIST_INDEX_ERROR,
    LIST_EMPTY_ERROR
} List_Error;

List_t List_new(void);
void List_delete(List_t self);
void List_deepDelete(List_t self, void Element_delete(void * element));
void * List_get(List_t self, unsigned int index, List_Error * error);
void * List_set(List_t self, unsigned int index, void * element, List_Error * error);
List_Error List_add(List_t self, unsigned int index, void * element);
void List_append(List_t self, void * element);
void * List_remove(List_t self, unsigned int index, List_Error * error);
size_t List_getSize(List_t self);
const char * List_errorStr(List_Error error);

#endif // LIST_H_INCLUDED
