#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int charVal(int integ){
    if(integ < 10) return '0' + integ;
    return 'A' + integ - 10;
}

int intVal(char ch){
    if(isupper(ch)) return ch - 'A' + 10;
    return ch - '0';
}

int isValid(char* number, int sourceBase, int destBase){
    int i, pointCount = 0;
    for(i = 0; number[i] != '\0'; i++){
        if(number[i] == '.'){
            if(pointCount == 1) return 0;
            pointCount++;
        }
        if(sourceBase > 36 || sourceBase < 2) return 0;
        if(destBase > 36 || destBase < 2) return 0;
        if(!pointCount && !isdigit(number[i]) && !isupper(number[i])) return 0;
        if(intVal(number[i]) >= sourceBase) return 0;
    }
    return 1;
}

long double convFloatPartToDec(char * number, unsigned int size, unsigned int sourceBase){
    long double sum = 0;
    int i;
    for(i = 0; i < size; i++){
        sum += intVal(number[i])*pow(sourceBase, -i - 1);
    }
    return sum;
}

char* ns_convert(char* number, unsigned int sourceBase, unsigned int destBase){
    int floatLen, intLen, strNumLen, isNeg = 0, intResLen = 60, flResLen = 13;/*flResLen considers '\0'*/
    char intRes[intResLen], flRes[flResLen];
    long int decI;
    long double decF;
    if(number[0] == '-'){
        isNeg = 1;
        number++;
    }
    if(!isValid(number, sourceBase, destBase)) return "";

    strNumLen = strlen(number);
    intLen = strcspn(number, ".");
    floatLen = strNumLen - intLen - 1;
    number[intLen] = '\0';

    decI = strtol(number, NULL, sourceBase);
    decF = convFloatPartToDec(number + intLen + 1, floatLen, sourceBase);

    intRes[intResLen - 1] = '\0';
    intRes[intResLen - 2] = '0';

    int i, left, intStart;
    if(decI != 0){
        for(i = intResLen - 2; decI != 0; i--){
            left = decI % destBase;
            intRes[i] = charVal(left);
            decI /= destBase;
        }
        intStart = i + 1;
    }else{ intStart = intResLen - 2;}

    int intP;
    for(i = 0; i < flResLen - 1; i++){
        decF *= destBase;
        intP = (int) decF;
        flRes[i] = charVal(intP);
        decF -= intP;
    }
    flRes[i] = '\0';

    if(decF == 0) sprintf(number, "%s", intRes + intStart);
    else sprintf(number, "%s.%s", intRes + intStart, flRes);

    if(isNeg) number--;
    return number;
}
