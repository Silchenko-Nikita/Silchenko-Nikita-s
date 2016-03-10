#include<stdlib.h>
#include<stdio.h>

#include "list.h"

//head is expected to be empty

static struct Node{
    List_Element element;
    struct Node * next;
};

static struct LinkedList{
    size_t size;
    struct Node * head;
};

typedef struct Node Node_s;
typedef struct LinkedList LinkedList_s;
typedef LinkedList_s * LinkedList_t;

List_t List_new(void){
    LinkedList_t list = (LinkedList_t)malloc(sizeof(LinkedList_s));
    Node_s * head = (Node_s *)malloc(sizeof(Node_s));
    if (NULL == list || NULL == head){
        puts("Failed to allocate memory");
        exit(1);
    }
    head->element = NULL;
    head->next = NULL;
    list->size = 0;
    list->head = head;
    return (List_t)list;
}

void List_delete(List_t list){
    LinkedList_t linked_list = (LinkedList_t)list;
    Node_s * curr = linked_list->head;

    free(linked_list);
    Node_s * next;
    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
}

List_Element List_get(List_t list, unsigned int index){
    Node_s * curr = ((LinkedList_t)list)->head;

    for(int i = 0; i <= index; i++){
        curr = curr->next;
        if(NULL == curr){
            return NULL;
        }
    }
    return (List_Element)curr->element;
}

List_ErrorStatus List_set(List_t list, unsigned int index, List_Element element){
    Node_s * curr = ((LinkedList_t)list)->head;

    for(int i = 0; i <= index; i++){
        curr = curr->next;
        if(NULL == curr){
            return LIST_INDEX_ERROR;
        }
    }
    curr->element = element;
    return LIST_OK;
}

List_ErrorStatus List_add(List_t list, unsigned int index, List_Element element){
    LinkedList_t linked_list = (LinkedList_t)list;
    Node_s * curr = linked_list->head;

    for(int i = 0; i < index; i++){
        curr = curr->next;
        if(NULL == curr){
            return LIST_INDEX_ERROR;
        }
    }

    Node_s * new_node = (Node_s *)malloc(sizeof(Node_s));
    if (NULL == new_node){
        puts("Failed to allocate memory");
        exit(1);
    }

    new_node->element = element;
    new_node->next = curr->next;
    curr->next = new_node;

    linked_list->size++;

    return LIST_OK;
}

List_ErrorStatus List_remove(List_t list, unsigned int index){
    LinkedList_t linked_list = (LinkedList_t)list;
    Node_s * curr = linked_list->head;

    Node_s * prev;
    for(int i = 0; i <= index; i++){
        prev = curr;
        curr = curr->next;
        if(NULL == curr){
            return LIST_INDEX_ERROR;
        }
    }

    prev->next = curr->next;
    free(curr);

    linked_list->size--;
    return LIST_OK;
}

void List_foreach(List_t list, void (*func)(List_Element element)){
    Node_s * curr = ((LinkedList_t)list)->head;

    while(curr->next != NULL){
        curr = curr->next;
        func(curr->element);
    }
}

List_Element List_t_max_el(List_t list, int (*cmp)(List_Element element1, List_Element element2)){
    Node_s * curr = ((LinkedList_t)list)->head;
    if (NULL == curr->next){
        return NULL;
    }

    curr = curr->next;
    Node_s * max = curr;
    while(curr->next != NULL){
        curr = curr->next;
        if(cmp(curr->element, max->element) > 0){
            max = curr;
        }
    }
    return (List_Element)max->element;
}

size_t List_getSize(List_t list){
    return ((LinkedList_t)list)->size;
}
