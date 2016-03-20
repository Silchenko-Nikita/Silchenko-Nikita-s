#ifndef PROC_SEQUENCE_H_INCLUDED
#define PROC_SEQUENCE_H_INCLUDED

#include "list.h"

typedef int intCB(int x);
typedef double doubleCB(double y);

void procSequence(List_t worldsList, intCB intCb, doubleCB doubleCb);

#endif // PROC_SEQUENCE_H_INCLUDED
