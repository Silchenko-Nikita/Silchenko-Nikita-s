#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

//describes universal list type for elements of any type

typedef void * List_t;
typedef void * List_Element;

typedef enum List_ErrorStatus{
    LIST_OK,
    LIST_INDEX_ERROR
} List_ErrorStatus;

List_t List_new(void);
void List_delete(List_t list);
List_Element List_get(List_t list, unsigned int index);
List_ErrorStatus List_set(List_t list, unsigned int index, List_Element element);
List_ErrorStatus List_add(List_t list, unsigned int index, List_Element element);
List_ErrorStatus List_remove(List_t list, unsigned int index);
void List_foreach(List_t list, void (*func)(List_Element element));
List_Element List_t_max_el(List_t list, int (*cmp)(List_Element element1, List_Element element2));
size_t List_getSize(List_t list);

#endif // LIST_H_INCLUDED
