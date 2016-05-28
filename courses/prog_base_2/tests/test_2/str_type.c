#include <ctype.h>
#include <strings.h>

#include "str_type.h"


int isAlpha(const char * str){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(!isalpha(str[i])) return 0;
    }
    return 1;
}

int isAlphaNum(const char * str){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(!isalnum(str[i])) return 0;
    }
    return 1;
}

int isNonNegatInteger(const char * str){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(!isdigit(str[i])) return 0;
    }
    return 1;
}

int isNonNegatReal(const char * str){
    int dot = 0;
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(str[i] == '.'){
             if(!dot) dot = 1;
             else return 0;
        } else if(!isdigit(str[i])) return 0;
    }
    return 1;
}

int isDate(const char * str){
    if(strlen(str) > 15) return 0;
    char buff[16];

    getTok(str, 0, "-", buff);
    if(!isNonNegatInteger(buff) || strlen(buff) != 4) return 0;

    getTok(str, 1, "-", buff);
    if(!isNonNegatInteger(buff) || strlen(buff) != 2 || atoi(buff) > 12) return 0;

    getTok(str, 2, "-", buff);
    if(!isNonNegatInteger(buff) || strlen(buff) != 2 || atoi(buff) > 31) return 0;

    getTok(str, 3, "-", buff);
    if(strlen(buff) != 0) return 0;

    return 1;
}

char * getTok(const char * str, unsigned int index, const char * delimeters, char * memPtr){
    char buff[500];
    strcpy(buff, str);

    int i = 0;
    for (char * tok = strtok(buff, delimeters); tok != NULL; i++, tok = strtok(NULL, delimeters)){
        if(i == index){
            strcpy(memPtr, tok);
            return memPtr;
        }
    }
    strcpy(memPtr, "");
    return memPtr;
}
