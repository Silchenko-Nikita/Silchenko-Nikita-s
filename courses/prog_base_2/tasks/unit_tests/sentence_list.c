#include<stdlib.h>
#include<stdio.h>

#include "sentence_list.h"

SentenceList_t SentenceList_new(void){
    return (SentenceList_t)List_new();
}

void SentenceList_delete(SentenceList_t list){
    List_delete(list);
}

Sentence_t SentenceList_get(SentenceList_t list, unsigned int index){
    return (Sentence_t)List_get(list, index);
}

SentenceList_ErrorStatus SentenceList_set(SentenceList_t list, unsigned int index, Sentence_t sentence){
    if (!Sentence_is_sentence(sentence)) return SENTENCE_LIST_NOT_SENTENCE;
    return (SentenceList_ErrorStatus)List_set(list, index, sentence);
}

SentenceList_ErrorStatus SentenceList_add(SentenceList_t list, unsigned int index, Sentence_t sentence){
    if (!Sentence_is_sentence(sentence)) return SENTENCE_LIST_NOT_SENTENCE;
    return (SentenceList_ErrorStatus)List_add(list, index, sentence);
}

SentenceList_ErrorStatus SentenceList_remove(SentenceList_t list, unsigned int index){
    return (SentenceList_ErrorStatus)List_remove(list, index);
}

size_t SentenceList_getSize(SentenceList_t list){
    return List_getSize(list);
}

Sentence_t SentenceList_get_longest_sent(SentenceList_t list){
    return (Sentence_t)List_t_max_el(list, Sentence_cmp_sent_length);
}

//you must free res's mem
char * SentenceList_get_longest_word(SentenceList_t list){
    return Sentence_get_longest_word(List_t_max_el(list, Sentence_cmp_longest_words_length));
}

void SentenceList_print(SentenceList_t list){
    puts("All sentences:");
    List_foreach(list, Sentence_print);
    puts("");
}
