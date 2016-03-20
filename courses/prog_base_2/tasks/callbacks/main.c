#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "proc_sequence.h"

int accmulateSumInt(int x){
    static int sum = 0;
    sum += x;
    return sum;
}

double accmulateSumDouble(double x){
    static double sum = 0;
    sum += x;
    return sum;
}

int accmulateAbsSumInt(int x){
    static int sum = 0;
    sum += abs(x);
    return sum;
}

double accmulateAbsSumDouble(double x){
    static double sum = 0;
    sum += fabs(x);
    return sum;
}

int main()
{
    List_t list = List_new();

    List_append(list, "1");
    List_append(list, "-2");
    List_append(list, "3");
    List_append(list, "-1.2");
    List_append(list, "1.1");

    procSequence(list, accmulateSumInt, accmulateSumDouble);
    procSequence(list, accmulateAbsSumInt, accmulateAbsSumDouble);

    printf("%d\n", accmulateSumInt(0));
    printf("%f\n\n", accmulateSumDouble(0));
    printf("%d\n", accmulateAbsSumInt(0));
    printf("%f", accmulateAbsSumDouble(0));

    List_delete(list);
    return 0;
}
