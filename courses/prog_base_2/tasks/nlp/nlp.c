#include <stdlib.h>
#include <strings.h>
#include <limits.h>

#include "nlp.h"

#include "list.h"

struct Word{
    char word[64];
};

struct Sentence{
    List_t words;
};

struct Text{
    List_t sentences;
    long int wordsNum;
};

Text_t Text_new(){
    Text_t self = (Text_t)malloc(sizeof(struct Text));
    self->sentences = List_new();
    self->wordsNum = 0;
    return self;
}

void Text_delete(Text_t self){
    List_deepDelete(self->sentences, Sentence_delete);
    free(self);
}

void Text_addSentence(Text_t self, unsigned int index, Sentence_t sentence){
    List_add(self->sentences, index, sentence);
    self->wordsNum += List_getSize(self->sentences);
}

Sentence_t Text_getSentence(Text_t self, unsigned int index){
    return List_get(self->sentences, index, NULL);
}

Sentence_t Text_removeSentence(Text_t self, unsigned int index){
    self->wordsNum -= List_getSize(self->sentences);
    return List_remove(self->sentences, index, NULL);
}

void Text_parse(Text_t self, const char * text){
    char * buff = (char *) malloc(LONG_MAX/10);
    strcpy(buff, text);
    char * toks[100000]; // sentences

    int toksNum = 0;
    for(char * tok = strtok(buff, "?!."); tok != NULL; tok = strtok(NULL, "?!.")){
        toks[toksNum] = tok;
        toksNum++;
    }

    for(int i = 0; i < toksNum; i++){
        Sentence_t sent = Sentence_new();
        Sentence_parse(sent, toks[i]);
        List_add(self->sentences, i, sent);
    }

    free(buff);
}

unsigned int Text_getSentNum(Text_t self){
    return List_getSize(self->sentences);
}

void Text_sortWords(Text_t self){
    Word_t words[100000];

    int k = 0;
    int sentsNum = List_getSize(self->sentences);
    for(int i = 0; i < sentsNum; i++){
        Sentence_t sent = Text_getSentence(self, i);
        int wordsNum = Sentence_getWordsNum(sent);
        for(int j = 0; j < wordsNum; j++){
            words[k] = Sentence_getWord(sent, j);
            k++;
        }
    }

    int compareWords(const void * * a, const void * * b){
        return strlen(Word_toString(*b)) - strlen(Word_toString(*a));
    }

    qsort(words, k, sizeof(char *), compareWords);

    k = 0;
    for(int i = 0; i < sentsNum; i++){
        Sentence_t sent = Text_getSentence(self, i);
        int wordsNum = Sentence_getWordsNum(sent);
        List_clear(sent->words);
        for(int j = 0; j < wordsNum; j++){
            Sentence_addWord(sent, j, words[k]);
            k++;
        }
    }
}

void Text_leaveWordWithSpecLen(Text_t self, unsigned int leftB, unsigned int rightB){
    int sentsNum = Text_getSentNum(self);
    self->wordsNum = 0;
    for(int i = 0; i < sentsNum; i++){
        Sentence_t sent = Text_getSentence(self, i);
        Sentence_leaveWordWithSpecLen(sent, leftB, rightB);
        self->wordsNum += Sentence_getWordsNum(sent);
    }
}

Sentence_t Sentence_new(){
    Sentence_t self = (Sentence_t)malloc(sizeof(struct Sentence));
    self->words = List_new();
    return self;
}

void Sentence_delete(Sentence_t self){
    List_deepDelete(self->words, Word_delete);
    free(self);
}

void Sentence_addWord(Sentence_t self, unsigned int index, Word_t word){
    List_add(self->words, index, word);
}

Word_t Sentence_getWord(Sentence_t self, int index){
    return List_get(self->words, index, NULL);
}

Word_t Sentence_removeWord(Sentence_t self, unsigned int index){
    return List_remove(self->words, index, NULL);
}

unsigned int Sentence_getWordsNum(Sentence_t self){
    return List_getSize(self->words);
}

void Sentence_parse(Sentence_t self, const char * sent){
    char buff[10000];
    strcpy(buff, sent);

    int i = 0;
    for(char * tok = strtok(buff, " (){}[],:.;-\"'\t\n"); tok != NULL; tok = strtok(NULL, " (){}[],:.;-\"'\t\n")){
        Word_t word = Word_new();
        Word_parse(word, tok);
        List_add(self->words, i, word);

        i++;
    }
}

void Sentence_leaveWordWithSpecLen(Sentence_t self, unsigned int leftB, unsigned int rightB){
    int wordsNum = Sentence_getWordsNum(self);
    int j = 0;
    for(int i = 0; i < wordsNum; i++, j++){
        int wordLen = strlen(Word_toString(Sentence_getWord(self, j)));

        if( wordLen < leftB || wordLen >= rightB ){
            Sentence_removeWord(self, j);

            j--;
        }
    }
}

Word_t Word_new(){
    Word_t self = (Word_t)malloc(sizeof(struct Word));
    strcpy(self->word, "");
    return self;
}

void Word_delete(Word_t self){
    free(self);
}

void Word_parse(Word_t self, const char * word){
    strcpy(self->word, word);
}

const char * Word_toString(Word_t self){
    return self->word;
}
