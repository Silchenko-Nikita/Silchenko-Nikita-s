#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED

#define PERSON_CATEGORIES_NUMBER 3

typedef struct Person * Person_t;

typedef enum {
    PERSON_STUDENT,
    PERSON_TEACHER,
    PERSON_PERSONNEL
} Person_Category;

typedef enum {
    PERSON_OK,
    PERSON_INVALID_FIRST_NAME,
    PERSON_INVALID_SECOND_NAME
} Person_Error;

Person_t Person_new(Person_Category category, const char * firstName, const char * secondName, Person_Error * error);
Person_t Person_newRandom(void);
void Person_delete(Person_t self);
Person_Category Person_getCategory(Person_t self);
const char * Person_getFirstName(Person_t self);
const char * Person_getSecondName(Person_t self);
const char * Person_categoryStr(Person_Category category);
const char * Person_errorStr(Person_Error error);

#endif // PERSON_H_INCLUDED
