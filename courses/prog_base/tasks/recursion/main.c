#include <stdio.h>
#include <stdlib.h>

int hexChToInt(char hex){
    if(hex >= '0' && hex <= '9'){
        return hex - '0';
    }else if(hex >= 'A' && hex <= 'F'){
        return hex - 'A' + 10;
    }else if(hex >= 'a' && hex <= 'f'){
        return hex - 'a' + 10;
    }else return -1;
}

int findFirst(char *p){
    if(hexChToInt(*p) != -1) return hexChToInt(*p);
    else if(*p == '\0') return -1;
    else return findFirst(p + 1);
}

int findLast(char *p, int last){
    if(*p == '\0') return last;
    else if(hexChToInt(*p) != -1){
        last = hexChToInt(*p);
        return findLast(p + 1, last);
    }else return findLast(p + 1, last);
}

int main()
{
    char string[100];
    printf("Please, enter some string: ");
    gets(string);
    int f = findFirst(string), l = findLast(string, -1);
    if(f == -1){
        printf("Invalid input!");
        return 1;
    }
    printf("Result: %d", f > l ? f : l);
    return 0;
}
