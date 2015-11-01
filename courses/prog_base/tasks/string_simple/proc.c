#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *process(char *resultStr, const char *textLines[], int linesNum, const char *extraStr){
    int i, isVowel, isNotVowel, vowelLine = -1, NotVowelLine = -1, k, check = 0;
    char *tok, vowelString[100], notVowelString[100], longer[100], buffer[100], l[100];
    for(i = 0; i < linesNum; i++){
        if(strlen(textLines[i]) == 0) continue;
        strcpy(buffer, textLines[i]);
        isVowel = 1;
        isNotVowel = 1;
        strcpy(l, textLines[i]);
        for(tok = strtok(l, " "); tok != NULL; tok = strtok(NULL, " ")){
            if(!(tok[0] >= 'A' && tok[0] <= 'Z') && !(tok[0] >= 'a' && tok[0] <= 'z')) {
                check = 1;
                break;
            }
            switch(tok[0]){
                case 'a':
                case 'e':
                case 'u':
                case 'o':
                case 'i':
                case 'y':
                case 'A':
                case 'E':
                case 'U':
                case 'O':
                case 'I':
                case 'Y':
                    isNotVowel = 0;
                    break;
                default:
                    isVowel = 0;
            }
        }

        if(check) continue;

        if(vowelLine == -1 && isVowel){
            vowelLine = i;
            strcpy(vowelString, buffer);
        }

        if(isNotVowel){
            NotVowelLine = i;
            strcpy(notVowelString, buffer);
        }
    }
    if(vowelLine == -1 && NotVowelLine == -1){
        strcpy(resultStr, "0 NULL ");
        strcat(resultStr, extraStr);
    } else{
        k = vowelLine + NotVowelLine;
        sprintf(resultStr, "%d ", k);
        strcat(resultStr, strlen(vowelString) > strlen(notVowelString) ? vowelString : notVowelString);
    }

    return resultStr;
}
