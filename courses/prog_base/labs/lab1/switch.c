#include <math.h>

int exec(int op, int a, int b) {
   int result;

   switch(op){
       case 0:
           result = -a;
           break;
        case 1:
           result = a + b;
           break;
        case 2:
           result = a - b;
           break;
        case 3:
           result = a*b;
           break;
        case 4:
           result = a/b;
           break;
        case 5:
           result = abs(a);
           break;
        case 6:
           result = pow(a, b);
           break;
        case 7:
        case 13:
        case 77:
            result = a % b;
            break;
        case 8:
           result = a > b ? a : b;
           break;
        case 9:
           result = a < b ? a : b;
           break;
        case 10:
            switch(abs(b) % 8){

                case 0:
                    result = abs(a) * sizeof(char);
                    break;
                case 1:
                    result = abs(a) * sizeof(signed char);
                    break;
                case 2:
                    result = abs(a) * sizeof(short);
                    break;
                case 3:
                    result = abs(a) * sizeof(unsigned int);
                    break;
                case 4:
                    result = abs(a) * sizeof(long);
                    break;
                case 5:
                    result = abs(a) * sizeof(unsigned long);
                    break;
                case 6:
                    result = abs(a) * sizeof(float);
                    break;
                case 7:
                    result = abs(a) * sizeof(double);
            }
            break;
        case 11:
            result = pow(a, 5 + b)/(1 + M_PI);
            break;
        default:
            if(op < 100){
                result = (op % abs(a + 1)) + (op % abs(b + 1));
            } else if(op >= 100){
                result = -1;
            }
   }
   return result;
}
