#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
    int N = 5;
    int a[N];
    int max, i;
    for(i = 0; i < N; i++){
        scanf("%d", &a[i]);
    }
    max = a[0];
    for(i = 0; i < N; i++){
        if (a[i] % 3 == 0){
            printf("%d", a[i]);
            if (!(i == N - 1)) printf(", ");
        }
        if (a[i] > max) max = a[i];
    }
    printf("\nMAX: %d", max);
    return 0;
}
