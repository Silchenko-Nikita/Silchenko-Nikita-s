#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fprocess(const char * pread, const char * pwrite){
    char t[50];
    FILE * f = fopen(pread, "r");
    FILE * f1 = fopen(pwrite, "w");
    if(NULL == f || NULL == f1){
        return 1;
    }
    int count = 0;
    while(count < 14){
        if('\n' == fgetc(f))
            count++;
    }
    fgets(t, 100, f);
    int a, b, c;

    if(strstr(t, "*")){
        a = atoi(strtok(t, "*"));
        b = atoi(strtok(NULL, "*"));
        c = a * b;
    }else if(strstr(t, "+")){
        a = atoi(strtok(t, "+"));
        b = atoi(strtok(NULL, "+"));
        c = a + b;
    }else if(strstr(t, "-")){
        a = atoi(strtok(t, "-"));
        b = atoi(strtok(NULL, "-"));
        c = a - b;
    }
    fprintf(f1, "%d", c);
    fclose(f);
    fclose(f1);
}
