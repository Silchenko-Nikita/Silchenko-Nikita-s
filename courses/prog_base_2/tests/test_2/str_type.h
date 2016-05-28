#ifndef STR_TYPE_H_INCLUDED
#define STR_TYPE_H_INCLUDED

#include "time.h"

int isAlpha(const char * str);
int isAlphaNum(const char * str);
int isNonNegatInteger(const char * str);
int isNonNegatReal(const char * str);
int isInteger(const char * str);
int isReal(const char * str);
int isDate(const char * str);

char * TimeToString(time_t time, char * mem_p);
char * getTok(const char * str, unsigned int index, const char * delimeters, char * memPtr);

#endif // STR_TYPE_H_INCLUDED
