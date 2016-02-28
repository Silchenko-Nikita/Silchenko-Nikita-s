#include <stdio.h>
#include <stdlib.h>

#include "sentence_list.h"

int main()
{

    SentenceList_ErrorStatus err;
    char * word;
    SentenceList_t list = SentenceList_new();

    SentenceList_add(list, 0, "m;m;lm;m.");
    SentenceList_add(list, 0, "m;m;lm;m.");
    SentenceList_add(list, 0, "m;m;lm;masddsaas1.");
    SentenceList_add(list, 3, "m;m; slm; m2");
    SentenceList_add(list, 2, "m;m;lm;m3.");
    SentenceList_remove(list, 3);
    SentenceList_set(list, 1, "How are you?");

    word = SentenceList_get_longest_word(list);
    puts(word);
    puts(SentenceList_get_longest_sent(list));
    puts("");
    SentenceList_print(list);

    free(word);
    SentenceList_delete(list);
    return 0;
}
