#include <stdlib.h>
#include <string.h>

#include "person.h"

static struct Person{
    Person_Category category;
    char firstName[16];
    char secondName[16];
};

typedef struct Person Person_s;

static const char * firstNameSamples[] = {
    "John",
    "Sara",
    "Lionel",
    "Gareth",
    "Valencia",
    "James",
    "Theodor",
    "Michail",
    "Lester",
    "Richard",
    "Albert",
    "Robert"
};

const int firstNameSamplesArrLen = 12;

static const char * secondNameSamples[] = {
    "Smith",
    "Jones",
    "Andersen",
    "Suarez",
    "Phillips",
    "Bale",
    "Gardner",
    "Brown",
    "Wilson",
    "Robinson",
    "Jackson",
    "Miller"
};

const int secondNameSamplesArrLen = 12;

Person_t Person_new(Person_Category category, const char * firstName, const char * secondName, Person_Error * error){
    int firstNameLen = strlen(firstName);
    int secondNameLen = strlen(secondName);
    if(firstNameLen == 0 || firstNameLen >= 16){
        if (error != NULL) *error = PERSON_INVALID_FIRST_NAME;
        return NULL;
    }

    if(secondNameLen == 0 || secondNameLen >= 16){
        if (error != NULL) *error = PERSON_INVALID_SECOND_NAME;
        return NULL;
    }

    Person_t self = (Person_t)malloc(sizeof(Person_s));
    if(NULL == self){
        puts("Error allrrrocating memory");
        exit(EXIT_FAILURE);
    }

    self->category = category;
    strcpy(self->firstName, firstName);
    strcpy(self->secondName, secondName);

    if (error != NULL) *error = PERSON_OK;
    return self;
}

Person_t Person_newRandom(void){
    return Person_new(rand() % PERSON_CATEGORIES_NUMBER, firstNameSamples[rand() % firstNameSamplesArrLen], secondNameSamples[rand() % secondNameSamplesArrLen], NULL);
}

void Person_delete(Person_t self){
    if(NULL == self) return;

    free(self);
}

Person_Category Person_getCategory(Person_t self){
    return self->category;
}

const char * Person_getFirstName(Person_t self){
    return self->firstName;
}

const char * Person_getSecondName(Person_t self){
    return self->secondName;
}

const char * Person_categoryStr(Person_Category category){
    static const char * repr[] = {
        "student",
        "teacher",
        "personnel"
    };

    return repr[category];
}

const char * Person_errorStr(Person_Error error){
    static const char * repr[] = {
        "OK",
        "Invalid first name",
        "Invalid second name"
    };

    return repr[error];
}
