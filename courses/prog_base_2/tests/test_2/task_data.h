#ifndef TASK_DATA_H_INCLUDED
#define TASK_DATA_H_INCLUDED

#include "time.h"

typedef struct TaskData{
    char author[128];
    char book[128];
    time_t time;
} * TaskData_t;

TaskData_t TaskData_new();
void TaskData_delete(TaskData_t self);

void TaskData_getFromXmlStr(TaskData_t self, const char * xmlStr);
char * TaskData_toXmlStr(TaskData_t self, const char * mem_p);

#endif // TASK_DATA_H_INCLUDED
