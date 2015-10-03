#include <stdio.h>
#include <stdlib.h>

int main()
{
    int h = 3, m = 13, code = 62;
    float grn;
    if(code < 10 || code > 999) return 1;
    switch(code){
        case 32:
        grn = 0.95*(h*60 + m);
        break;
        case 44:
        grn = 0.77*(h*60 + m);
        break;
        case 62:
        grn = 0.8*(h*60 + m);
        break;
        case 697:
        case 692:
        grn = 1.5*(h*60 + m);
        break;
        default:
        grn = (h*60 + m);
    }
    printf("%.2f", grn);
    return 0;
}
