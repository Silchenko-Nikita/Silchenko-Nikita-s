#ifndef SENTENCE_H_INCLUDED
#define SENTENCE_H_INCLUDED

typedef char * Sentence_t;

int Sentence_is_sentence(char * text);
int Sentence_get_length(char * text);
char * Sentence_get_longest_word(Sentence_t sentence); //you must free res's mem
int Sentence_cmp_sent_length(Sentence_t sent1, Sentence_t sent2);
int Sentence_cmp_longest_words_length(Sentence_t sent1, Sentence_t sent2);
void Sentence_print(Sentence_t sentence);

#endif // SENTENCE_H_INCLUDED
