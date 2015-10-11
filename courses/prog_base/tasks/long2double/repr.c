#include <stdlib.h>
#include <math.h>

double long2double(long long a){
    char bin[64];
    int i, E = 0;
    double result = 0;
    if (a < 0){
        unsigned long long a1 = a + (long long) powl(2, 63);
        bin[0] = '1';
        for (i = 63; i > 0; i--){
            bin[i] = a1 % 2 ? '1' : '0';
            a1 /= 2;
        }
    }else {
        bin[0] = '0';
        for (i = 63; i > 0; i--){
            bin[i] = a % 2 ? '1' : '0';
            a /= 2;
        }
    }

    for (i = 0; i < 11; i++){
        E += bin[11 - i] == '1' ? powl(2, i) : 0 ;
    }

    for (i = 0; i < 64; i++){
            printf("%c\n", bin[i]);
        }

    if (E == 0){
        for (i = 12; i < 64; i++){
           result += bin[i] == '1' ? pow(2, (-i + 11) - 1022) : 0;
        }
    }else if (E == 2047){
        for (i = 12; i < 64; i++){
           if (bin[i] == '1'){
                result = NAN;
                goto point;
           }
        }
        result = INFINITY;
    }else{
        for (i = 12; i < 64; i++){
           result += bin[i] == '1' ? pow(2, (-i + 11) + (E - 1023)) : 0;
        }
        result += pow(2, (E - 1023));
    }

    point:
    if (bin[0] == '1') result = -result;
    return result;
}
