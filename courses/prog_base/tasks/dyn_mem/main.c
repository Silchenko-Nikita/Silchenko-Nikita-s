#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    char * string;
    string = (char *) malloc(100*sizeof(char));
    printf("Please, enter some string: ");
    gets(string);
    int *f, *l, *i;
    f = (int *) malloc(sizeof(int));
    l = (int *) malloc(sizeof(int));
    i = (int *) malloc(sizeof(int));
    *f = -1;
    *l = -1;
    for(*i = 0; string[*i] != '\0'; (*i)++){
        if(isdigit(string[*i])){
            if(*f == -1){
                *f = string[*i] - '0';
            }
            *l = string[*i] - '0';
        }
    }
    if(*f == -1){
        printf("Invalid input!");
        return 1;
    }
    printf("Result: %d", *f - *l);
    return 0;
}
