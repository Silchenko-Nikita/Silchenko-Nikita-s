#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Patient{unsigned int age; char name[50], diagnoses[50]};

int count(int size, struct Patient arr[size]){
    int i, c = 0;
    for(i = 0; i < size; i++){
        if(arr[i].age < 18) c++;
    }
    return c;
}

void change(struct Patient * pVar, const char * newValue){
    strcpy(pVar->diagnoses, newValue);
}

void print(struct Patient * pVar){
    printf("Name: %s\nAge: %d\nDiagnoses: %s\n\n", pVar->name, pVar->age, pVar->diagnoses);
}


int main()
{
    struct Patient patients[4] = {{38, "John Cena", "malaria"}, {70, "Dart Vader", "flu"}, {17, "Marty McFly", "brain cancer"}, {2, "Stuart Little", "asthma"}};
    int i, ind;
    char diag[50];
    for(i = 0; i < 4; i++){
        printf("Index: %d\n", i);
        print(patients + i);
    }
    printf("Number of patient under 18 years old: %d\n\n", count(4, patients));
    printf("Please, enter index: ");
    gets(diag);
    ind = atoi(diag);
    puts("");
    printf("Please, enter new diagnoses: ");
    gets(diag);
    change(patients + ind, diag);
    puts("\nChanged:");
    print(patients + ind);
    return 0;
}
