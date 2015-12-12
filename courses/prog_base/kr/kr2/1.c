#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * findLongestWord(char * string){
    int maxLen = 0, len;
    char *l, *maxLenStr = NULL;
    for(l = strtok(string, " \n\t"); l != NULL; l = strtok(NULL, " \n\t")){
        if((len = strlen(l)) > maxLen){
            maxLen = len;
            maxLenStr = l;
        }
    }
    return maxLenStr;
}

void fprintRes(FILE * otputFile, char * string, int strNum){
    fprintf(otputFile, "First longest world in %dth string is: %s\n", strNum, string);

}

void parseFile(char * inputFileName, char * outputFileName){
    FILE * inputFile = fopen(inputFileName, "r");
    FILE * outputFile = fopen(outputFileName, "w");
    if(NULL == inputFile || NULL == outputFile){
        puts("Error opening file");
        exit(1);
    }
    char string[200];
    int i;
    for(i = 0; !feof(inputFile); i++){
        fprintRes(outputFile, findLongestWord(fgets(string, 200, inputFile)), i);
    }
    fclose(inputFile);
    fclose(outputFile);
}

int main(int argc, char * argv[])
{
    if(argc < 3){
        puts("Not enough string arguments");
        return 1;
    }
    parseFile(argv[1], argv[2]);
    return 0;
}
