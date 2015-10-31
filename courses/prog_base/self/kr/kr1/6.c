#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void print(int a, int b){
    printf("AND: %d\n", a & b);
    printf("OR: %d\n", a | b);
    printf("XOR: %d", a ^ b);
}

int main()
{
    char a[17], b[17];
    unsigned int a1 = 0, b1 = 0;
    int i;
    scanf("%s", a);
    scanf("%s", b);
    for(i = 16; i >= 0; i--){
        if ((a[i] != '1' && a[i] != '0') || (b[i] != '1' && b[i] != '0')){
            printf("Exception");
            return 1;
        }
        if (a[i] == '1') a1 += powl(2, 16 - i);
        if (b[i] == '1') b1 += powl(2, 16 - i);
    }
    print(a1, b1);
    return 0;
}
