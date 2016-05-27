#include "list.h"

static const size_t arrayInitialCapacity = 8;

static struct List{
    void ** array;
    size_t arrayCapacity;
    size_t size
};

typedef struct List List_s;

List_t List_new(void){
    List_t self = (List_t)malloc(sizeof(List_s));
    if(NULL == self){
        puts("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    self->array = (void **)malloc(sizeof(void *) * arrayInitialCapacity);
    if(NULL == self->array){
        puts("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    self->arrayCapacity = arrayInitialCapacity;
    self->size = 0;

    return self;
}

void List_delete(List_t self){
    if(NULL == self) return;

    free(self->array);
    free(self);
}

void List_deepDelete(List_t self, void Element_delete(void * element)){
    if(NULL == self) return;

    for(int i = 0; i < self->size; i++){
        Element_delete(self->array[i]);
    }
    free(self->array);
    free(self);
}

void * List_get(List_t self, unsigned int index, List_Error * error){
    if(index >= self->size){
        if(error != NULL) *error = LIST_INDEX_ERROR;
        return NULL;
    }

    if(error != NULL) *error = LIST_OK;
    return self->array[index];
}

void * List_set(List_t self, unsigned int index, void * element, List_Error * error){
    if(index >= self->size){
        if(error != NULL) *error = LIST_INDEX_ERROR;
        return NULL;
    }

    void * old = self->array[index];
    self->array[index] = element;

    if(error != NULL) *error = LIST_OK;
    return old;
}

List_Error List_add(List_t self, unsigned int index, void * element){
    if(index > self->size){
        return LIST_INDEX_ERROR;
    }

    if(self->arrayCapacity == self->size){
        void ** oldArray = self->array;
        self->arrayCapacity *= 2;
        self->array = (void **)malloc(sizeof(void *) * self->arrayCapacity);
        if(NULL == self->array){
            puts("Error allocating memory");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < index; i++){
            self->array[i] = oldArray[i];
        }
        self->array[index] = element;
        for(int i = index; i < self->size; i++){
            self->array[i + 1] = oldArray[i];
        }

        free(oldArray);
    }else{
        for(int i = self->size; i > index; i--){
            self->array[i] = self->array[i - 1];
        }
        self->array[index] = element;
    }

    self->size++;

    return LIST_OK;
}

void List_append(List_t self, void * element){
    List_add(self, self->size, element);
}

void * List_remove(List_t self, unsigned int index, List_Error * error){
    if(self->size == 0){
        if(error != NULL) *error = LIST_EMPTY_ERROR;
        return NULL;
    }

    if(index >= self->size){
        if(error != NULL) *error = LIST_INDEX_ERROR;
        return NULL;
    }

    void * removedEl = self->array[index];
    for(int i = index; i < self->size - 1; i++){
        self->array[i] = self->array[i + 1];
    }

    self->size--;

    if(error != NULL) *error = LIST_OK;
    return removedEl;
}

size_t List_getSize(List_t self){
    return self->size;
}

const char * List_errorStr(List_Error error){
    static const char * repr[] = {
        "OK",
        "Index error",
        "Empty list error"
    };
    return repr[error];
}
