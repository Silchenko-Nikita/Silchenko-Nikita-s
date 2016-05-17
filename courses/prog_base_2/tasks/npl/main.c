#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <limits.h>

#include "npl.h"
#include "list.h"

int main()
{
    FILE * inputFile = fopen("input.txt", "r");
    FILE * outputFile = fopen("output.txt", "w");
    if(NULL == inputFile){
        puts("error opening file text.txt!");
        return 0;
    }

    char * strText = (char *) malloc(LONG_MAX/10);

    for(int i = 0; !feof(inputFile); i++){
        strText[i] = getc(inputFile);
    }

    Text_t text = Text_new();
    Text_parse(text, strText);

    Text_leaveWordWithSpecLen(text, 5, INFINITY);

    Text_sortWords(text);

    int sentsNum = Text_getSentNum(text);
    for(int i = 0; i < sentsNum; i++){
        Sentence_t sent = Text_getSentence(text, i);
        int wordsNum = Sentence_getWordsNum(sent);
        for(int j = 0; j < wordsNum; j++){
            fputs(Word_toString(Sentence_getWord(sent, j)), outputFile);
            putc('\n', outputFile);
        }
    }

    Text_delete(text);
    free(strText);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
