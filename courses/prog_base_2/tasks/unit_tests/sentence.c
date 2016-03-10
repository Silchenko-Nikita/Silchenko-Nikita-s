#include <string.h>
#include <ctype.h>

#include "sentence.h"

int Sentence_is_sentence(char * text){
    return strchr("!?.", text[strlen(text) - 1]);
}

int Sentence_get_length(char * text){
    if (!Sentence_is_sentence(text)) return -1;
    return strlen(text);
}

//you must free res's mem
char * Sentence_get_longest_word(Sentence_t sentence){
    int word_started = 0;
    int curr_length;
    int max_length = 0;
    char * curr_word;
    char * max_length_word;

    for(char * ch = sentence; ch[0] != '\0'; ch++){
        if (word_started){
            if (isalpha(ch[0])){
                curr_length++;
            }else{
                word_started = 0;
                if (max_length < curr_length){
                    max_length = curr_length;
                    max_length_word = curr_word;
                }
            }
        } else{
            if (isalpha(ch[0])){
                curr_word = ch;
                word_started = 1;
                curr_length = 1;
            }
        }
    }

    curr_word = malloc((max_length + 1)*sizeof(char));
    if (NULL == curr_word){
        puts("Failed to allocate memory");
        exit(1);
    }
    memcpy(curr_word, max_length_word, max_length*sizeof(char));
    curr_word[max_length] = '\0';
    return curr_word;
}

int Sentence_cmp_sent_length(Sentence_t sent1, Sentence_t sent2){
    return strlen(sent1) - strlen(sent2);
}

int Sentence_cmp_longest_words_length(Sentence_t sent1, Sentence_t sent2){
    char * word1 = Sentence_get_longest_word(sent1);
    char * word2 = Sentence_get_longest_word(sent2);
    int res = strlen(word1) - strlen(word2);
    free(word1);
    free(word2);
    return res;
}

void Sentence_print(Sentence_t sentence){
    puts(sentence);
}
