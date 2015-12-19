#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define NAME_CAPAS 20
#define TEXT_CAPAS 100
#define INPUT_X 30
#define INPUT_Y 0
#define INVALID_INPUT_TIME 1200

typedef struct TxtFile{char name[NAME_CAPAS], text[TEXT_CAPAS]; struct Folder * parent} TxtFile;
typedef struct Folder{
    char name[NAME_CAPAS];
    struct Folder * parent;
    int foldersNum; struct Folder * folders;
    int txtFilesNum; TxtFile * txtFiles;
} Folder;

const char * const serializeFileName = "serialize.bin";
const char * const basicFolderName = "rootFolder";

void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
}

void clearScreen() {
    system("cls");
}

Folder * findFolder(char * fullPath, Folder * base){
    char path[10*NAME_CAPAS], *d;
    strcpy(path, fullPath);
    int i, k;
    Folder * folder = base;
    for(d = strtok(path, "\\"); d != NULL; d = strtok(NULL, "\\")){
        k = folder->foldersNum;
        for(i = 0; i < k; i++){
            if(!strcmp(folder->folders[i].name, d)){
                folder = folder->folders + i;
                break;
            }
        }
        if(i == k) return NULL; /*if not found*/
    }
    return folder;
}

void getParsPathByFullPath(char * fullPath, char * parsPathP){
    int d = strlen(fullPath), i, j;
    char * l;
    l = fullPath + d;
    for(i = 0; i < d && *(--l + 1) != '\\'; i++){}
    for(j = 0; j < d - i; j++){
        *(parsPathP + j) = *(fullPath + j);
    }
    *(parsPathP + j) = '\0';
}

TxtFile * findTxtFile(char * fullPath, Folder * base){
    char path[10*NAME_CAPAS], d[NAME_CAPAS];
    getNameByPath(fullPath, d);
    getParsPathByFullPath(fullPath, path);
    Folder * parent = findFolder(path, base);
    int i;
    for(i = 0; i < parent->txtFilesNum; i++){
        if(!strcmp(parent->txtFiles[i].name, d)) return parent->txtFiles + i;
    }
    return NULL;
}

void getNameByPath(char * fullPath, char * nameP){
    int d = strlen(fullPath), i;
    char * name = fullPath + d;
    for(i = 0; i < d && *(name - 1) != '\\'; i++){name--;}
    strcpy(nameP, name);
}

int folderInd(Folder * folder){
    int i;
    for(i = 0; i < folder->parent->foldersNum; i++){
        if(!strcmp(folder->parent->folders[i].name, folder->name)) return i;
    }
    return -1;
}

int txtFileInd(TxtFile * txtFile){
    int i;
    for(i = 0; i < txtFile->parent->txtFilesNum; i++){
        if(!strcmp(txtFile->parent->txtFiles[i].name, txtFile->name))  return i;
    }
    return -1;
}

void addFolder(char * fullPath, Folder * base){
    char parsPath[10*NAME_CAPAS], newFolderName[NAME_CAPAS];
    getNameByPath(fullPath, newFolderName);
    getParsPathByFullPath(fullPath, parsPath);
    Folder * parent = findFolder(parsPath, base);
    if(NULL == parent || findFolder(fullPath, base) != NULL){
        invalidInput();
        return;
    }

    if(parent->foldersNum == 0){
        parent->folders = (Folder *)malloc(2*sizeof(Folder));
        if(NULL == parent->folders) exit(1);
    }else if(parent->foldersNum % 2 == 0){
        parent->folders = (Folder *)realloc(parent->folders, sizeof(Folder)*parent->foldersNum + 2*sizeof(Folder));
        if(NULL == parent->folders) exit(1);
    }

    initFolder(parent->folders + parent->foldersNum++, newFolderName, parent);
}

void printFolder(Folder * folder, int x, int * y){
    moveCursor(x, *y);
    printf(folder->name);
    int i;
    for(i = 0; i < folder->foldersNum; i++){
        (*y)++;
        printFolder(folder->folders + i, x + 1, y);
    }
    for(i = 0; i < folder->txtFilesNum; i++){
        (*y)++;
        moveCursor(x + 1, *y);
        printf(folder->txtFiles[i].name);
    }
}

void freeFolderMem(Folder * base){
    int i;
    for(i = 0; i < base->foldersNum; i++){
        freeFolderMem(base->folders + i);
    }
    if(base->folders != NULL)
        free(base->folders);
    if(base->txtFiles != NULL)
        free(base->txtFiles);
}

void renameFolder(char * fullPath, Folder * base, char * newName){
    Folder * toRename = findFolder(fullPath, base);
    if(NULL == toRename){
        invalidInput();
        return;
    }

    strcpy(toRename->name, newName);
}

void renameTxtFile(char * fullPath, Folder * base, char * newName){
    TxtFile * toRename = findTxtFile(fullPath, base);
    if(NULL == toRename){
        invalidInput();
        return;
    }

    strcpy(toRename->name, newName);
}

void removeFolder(char * fullPath, Folder * base){
    Folder * toRemove = findFolder(fullPath, base);
    if(NULL == toRemove){
        invalidInput();
        return;
    }

    int i;
    freeFolderMem(toRemove);

    int k = folderInd(toRemove);
    Folder * foldersArr = toRemove->parent->folders;
    int foldersArrSize = --toRemove->parent->foldersNum;
    for(; k < foldersArrSize; k++){
        foldersArr[k] = foldersArr[k + 1];
    }
}

void removeTxtFile(char * fullPath, Folder * base){
    TxtFile * toRemove = findTxtFile(fullPath, base);
    if(NULL == toRemove){
        invalidInput();
        return;
    }

    int k = txtFileInd(toRemove);
    TxtFile * txtFilesArr = toRemove->parent->txtFiles;
    int txtFilesArrSize = --toRemove->parent->txtFilesNum;
    for(; k < txtFilesArrSize; k++){
        txtFilesArr[k] = txtFilesArr[k + 1];
    }
}

void printTextInTxtFile(char * fullPath, Folder * base, int x, int y){
    TxtFile * txtFile = findTxtFile(fullPath, base);
    if(NULL == txtFile){
        invalidInput();
        return;
    }

    moveCursor(x, y);
    printf(txtFile->text);
}

void addTxtFile(char * fullPath, Folder * base){
    char parsPath[10*NAME_CAPAS], newTxtFileName[NAME_CAPAS];
    getNameByPath(fullPath, newTxtFileName);
    getParsPathByFullPath(fullPath, parsPath);
    Folder * parent = findFolder(parsPath, base);
    if(NULL == parent || findTxtFile(fullPath, base) != NULL){
        invalidInput();
        return;
    }

    if(parent->txtFilesNum == 0){
        parent->txtFiles = (TxtFile *)malloc(2*sizeof(TxtFile));
        if(NULL == parent->txtFiles) exit(1);
    }else if(parent->txtFilesNum % 2 == 0){
        parent->txtFiles = (TxtFile *)realloc(parent->txtFiles, sizeof(TxtFile)*parent->txtFilesNum + 2*sizeof(TxtFile));
        if(NULL == parent->txtFiles) exit(1);
        int i, j;
        for(i = 0; i < parent->foldersNum; i++){
            for(j = 0; j < parent->folders[i].foldersNum; i++){
                parent->folders[i].folders[j].parent = parent->folders + i;
            }
        }
    }

    initTxtFile(parent->txtFiles + parent->txtFilesNum++, newTxtFileName, parent);
}


void initFolder(Folder * folder, char * name, Folder * parent){
    strcpy(folder->name, name);
    folder->foldersNum = 0;
    folder->txtFilesNum = 0;
    folder->parent = parent;
    folder->folders = NULL;
    folder->txtFiles = NULL;
}

void initTxtFile(TxtFile * txtFile, char * name, Folder * parent){
    strcpy(txtFile->name, name);
    txtFile->parent = parent;
    txtFile->text[0] = '\0';
}

void printFolder_simple(Folder * folder, int x, int y){
    printFolder(folder, x, &y);
}

void serializeFolder(Folder * folder, FILE * file){
    fwrite(folder, sizeof(Folder), 1, file);
    int i;
    for(i = 0; i < folder->foldersNum; i++){
        serializeFolder(folder->folders + i, file);
    }
    for(i = 0; i < folder->txtFilesNum; i++){
        fwrite(folder->txtFiles + i, sizeof(TxtFile), 1, file);
    }
}

void deserializeFolder(Folder * folder, Folder * parent, FILE * file){
    fread(folder, sizeof(Folder), 1, file);
    folder->parent = parent;
    folder->folders = (Folder *) malloc(2*sizeof(Folder)*folder->foldersNum);
    int i;
    for(i = 0; i < folder->foldersNum; i++){
        deserializeFolder(folder->folders + i, folder, file);
    }
    folder->txtFiles = (TxtFile *) malloc(2*sizeof(TxtFile)*folder->txtFilesNum);

    for(i = 0; i < folder->txtFilesNum; i++){
        fread(folder->txtFiles + i, sizeof(TxtFile), 1, file);
        folder->txtFiles[i].parent = folder;
    }
}

bool deserializeFolder_simple(Folder * folder, Folder * parent){
    FILE * file = fopen(serializeFileName, "rb");
    if(NULL == file) return false;
    deserializeFolder(folder, parent, file);
    fclose(file);
    return true;
}

void serializeFolder_simple(Folder * basicFolder){
    FILE * file = fopen(serializeFileName, "wb");
    if(NULL == file){
        freeFolderMem(basicFolder);
        exit(1);
    }
    serializeFolder(basicFolder, file);
    fclose(file);
}

bool isTxtFile(char * fullPath){
    if(NULL == fullPath) return false;
    int len = strlen(fullPath);
    if(len < 5) return false;
    char *t;
    t = fullPath + strlen(fullPath);
    t -= 4;
    if(!strcmp(t, ".txt")) return true;
    return false;
}

void invalidInput(){
    moveCursor(INPUT_X, 2);
    printf("Invalid input!");
    Sleep(INVALID_INPUT_TIME);
}

void printHelp(){
    clearScreen();
    puts("Description of commands:\n");
    puts("\"add !folderPath!\" : to add folder");
    puts("\"add !txtFilePath!.txt\" : to add txt file");
    puts("\"remove !folderPath!\" : to remove folder");
    puts("\"remove !txtFilePath!.txt\" : to remove txt file");
    puts("\"rename !folderPath! !newName!\" : to rename folder");
    puts("\"rename !txtFilePath!.txt !newName!.txt\" : to rename txt file");
    puts("\"set content !txtFilePath!.txt\" : to set txt file content");
    puts("\"print content !txtFilePath!.txt\" : to print txt file content");
    puts("\"exit\" : to serialize data and exit the program");
    puts("_______");
    puts("You don't need to specify rootFolder name while typing path");
    puts("Type path in format: !folder1Name!\\!folder2Name!\\...\\!fileName!\n");
}

bool getCommand(Folder * base){
    char *comm, *atr, got[10*NAME_CAPAS];
    gets(got);
    comm = strtok(got, " ");
    atr = strtok(NULL, " ");

    if(NULL == comm){
        invalidInput();
        return false;
    }

    if(!strcmp(comm, "exit")){
        return true;
    }else if(!strcmp(comm, "help")){
        printHelp();
        printf("Press any key to continue..");
        getch();
        return false;
    }

    if(NULL == atr || isspace(atr[0])){
        invalidInput();
        return false;
    }

    if(!strcmp(comm, "add")){
        if(isTxtFile(atr)){
            addTxtFile(atr, base);
        }else{
            addFolder(atr, base);
        }
    }else if(!strcmp(comm, "remove")){
        if(isTxtFile(atr)){
            removeTxtFile(atr, base);
        }else{
            removeFolder(atr, base);
        }
    }else if(!strcmp(comm, "rename")){
        if(isTxtFile(atr)){
            char * s = strtok(NULL, " ");
            if(!isTxtFile(s)){
                invalidInput();
                return false;
            }
            renameTxtFile(atr, base, s);
        }else{
            char * s = strtok(NULL, " ");
            if(isTxtFile(s)){
                invalidInput();
                return false;
            }
            renameFolder(atr, base, s);
        }
    }else if(!strcmp(comm, "set") && !strcmp(atr, "content")){
        atr = strtok(NULL, " ");
        if(findTxtFile(atr, base) == NULL){
            invalidInput();
            return false;
        }
        moveCursor(INPUT_X, INPUT_Y + 3);
        printf("Write new content here:");
        moveCursor(INPUT_X, INPUT_Y + 4);
        gets(findTxtFile(atr, base)->text);
    }else if(!strcmp(comm, "print") && !strcmp(atr, "content")){
        atr = strtok(NULL, " ");
        if(findTxtFile(atr, base) == NULL){
            invalidInput();
            return false;
        }
        moveCursor(INPUT_X, INPUT_Y + 3);
        printf("The content of %s:", atr);
        printTextInTxtFile(atr, base, INPUT_X, INPUT_Y + 4);
        moveCursor(INPUT_X, INPUT_Y + 6);
        printf("Press any key to continue..");
        getch();
    }else{
        invalidInput();
    }
    return false;
}

int main(){
    Folder basicFolder;
    if(!deserializeFolder_simple(&basicFolder, NULL)){
        initFolder(&basicFolder, basicFolderName, NULL);
    }

    while(true){
        clearScreen();
        printFolder_simple(&basicFolder, 0, 0);
        moveCursor(INPUT_X, INPUT_Y);
        printf("Enter your command here(enter \"help\" to get help):");
        moveCursor(INPUT_X, INPUT_Y + 1);
        if (getCommand(&basicFolder)) break;
        moveCursor(0, 0);
    }
    clearScreen();
    serializeFolder_simple(&basicFolder);
    freeFolderMem(&basicFolder);
    moveCursor(0, 0);
    return 0;
}
