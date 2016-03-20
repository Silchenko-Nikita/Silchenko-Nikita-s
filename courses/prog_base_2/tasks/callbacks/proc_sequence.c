#include "proc_sequence.h"

static int isInt(const char * word){
    if (word[0] == '-') word++;

    for(char * chP = word; *chP != '\0'; chP++){
        if(*chP > '9' || *chP < '0') return 0;
    }

    return 1;
}

static int isFloat(const char * word){
    if (word[0] == '-') word++;

    int pointFlag = 0;
    for(char * chP = word; *chP != '\0'; chP++){
        if(*chP == '.'){
            if(!pointFlag && chP[1] != '\0') pointFlag = 1;
            else return 0;
        } else if(*chP > '9' || *chP < '0') return 0;
    }

    if(pointFlag) return 1;
    else return 0;
}

void procSequence(List_t worldsList, intCB intCb, doubleCB doubleCb){
    int listSize = List_getSize(worldsList);
    for(int i = 0; i < listSize; i++){
        char * word = List_get(worldsList, i, NULL);
        if(isInt(word) && intCb != NULL) intCb(atoi(word));
        if(isFloat(word) && doubleCb != NULL) doubleCb(atof(word));
    }
}
