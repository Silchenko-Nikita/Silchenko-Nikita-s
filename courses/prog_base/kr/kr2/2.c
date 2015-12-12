#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_DRAGON_SIZE 20
#define DEFAULT_FILM_YEAR 2015

typedef struct Dragon{char name[100]; int size} Dragon;
typedef struct Film{char name[100]; int year; Dragon * dragon} Film;

void initDragon(Dragon * dragon, int pos){
    sprintf(dragon->name, "dragon #%d", pos);
    dragon->size = DEFAULT_DRAGON_SIZE;
}

void initFilm(Film * film, int pos){
    sprintf(film->name, "film #%d", pos);
    film->year = DEFAULT_FILM_YEAR;
}

Dragon * allocDragon(Dragon * dragon, int pos){
    dragon = (Dragon *)malloc(sizeof(Dragon));
    return dragon;
}

Film * allocFilm(Film * film, int pos){
    film = (Film *)malloc(sizeof(Film));
    return film;
}

void deleteDragon(Dragon * dragon){
    free(dragon);
}

void deleteFilm(Film * film){
    free(film);
}

int cmpDragons(Dragon * dragon1, Dragon * dragon2){
    return strcmp(dragon1->name, dragon2->name);
}

int cmpFilms(Film * film1, Film * film2){
    return strcmp(film1->name, film2->name);
}

void printDragon(Dragon * dragon){
    printf("dragons name %s\n", dragon->name);
    printf("dragons size %d\n\n", dragon->size);
}

void printFilm(Film * film){
    printf("films name %s\n", film->name);
    printf("films year %d\n\n", film->year);
}

void printDragonsArr(int dragonsNum, Dragon dragons[dragonsNum]){
    int i;
    for(i = 0; i < dragonsNum; i++){
        printDragon(dragons + i);
    }
}

void printFilmsArr(int filmsNum, Film films[filmsNum]){
    int i;
    for(i = 0; i < filmsNum; i++){
        printFilm(films + i);
    }
}

void printDragonsByRule(int filmsNum, Film films[filmsNum]){
    int i;
    for(i = 0; i < filmsNum; i++){
        if(films[i].dragon->size > 20 && DEFAULT_FILM_YEAR - films[i].year <= 20)
            printDragon(films[i].dragon);
    }
}

void getDragon(Dragon * dragon){
    char getS[100], getI[10];
    puts("Type dragons name:");
    gets(getS);
    puts("Type dragons size:");
    gets(getI);
    int k = atoi(getI);
    if(k < 0){
        printf("can't be negative\n\n");
        return;
    }
    strcpy(dragon->name, getS);
    dragon->size = atoi(getI);
    puts("");
}

void getFilm(Film * film){
    char getS[100], getI[10];
    puts("Type films name:");
    gets(getS);
    puts("Type films year:");
    gets(getI);
    int k = atoi(getI);
    if(k < 0){
        printf("can't be less then 1900\n\n");
        return;
    }
    if(k > 2015){
        printf("can't be bigger then 2015\n\n");
        return;
    }
    strcpy(film->name, getS);
    film->year = atoi(getI);
    puts("");
}


int main()
{
    Dragon dragons[30];
    int dragonsNum = 0;
    Film films[30];
    int filmsNum = 0;
    /*getDragon(dragons);
    getDragon(dragons + 1);
    getDragon(dragons + 2);*/
    initDragon(dragons, 1);
    printDragonsArr(1, dragons);
    return 0;
}

