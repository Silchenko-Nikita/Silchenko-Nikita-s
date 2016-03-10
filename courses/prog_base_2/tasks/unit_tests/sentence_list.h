#ifndef SENTENCE_LIST_H_INCLUDED
#define SENTENCE_LIST_H_INCLUDED

//describes specific list type only for elements of sentence type

#include "sentence.h"
#include "list.h"

typedef List_t SentenceList_t;

typedef enum SentenceList_ErrorStatus{
    SENTENCE_LIST_OK,
    SENTENCE_LIST_INDEX_ERROR,
    SENTENCE_LIST_NOT_SENTENCE
} SentenceList_ErrorStatus;

SentenceList_t SentenceList_new(void);
void SentenceList_delete(SentenceList_t list);
Sentence_t SentenceList_get(SentenceList_t list, unsigned int index);
SentenceList_ErrorStatus SentenceList_set(SentenceList_t list, unsigned int index, Sentence_t sentence);
SentenceList_ErrorStatus SentenceList_add(SentenceList_t list, unsigned int index, Sentence_t sentence);
SentenceList_ErrorStatus SentenceList_remove(SentenceList_t list, unsigned int index);
size_t SentenceList_getSize(SentenceList_t list);

Sentence_t SentenceList_get_longest_sent(SentenceList_t list);
char * SentenceList_get_longest_word(SentenceList_t list); //you must free res's mem
void SentenceList_print(SentenceList_t list);

#endif // SENTENCE_LIST_H_INCLUDED
