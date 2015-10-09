#include <math.h>

int satisfies(int a, int b, int c) {
   int result;
   int max, min, modmin, sum2, bla;
   int count = a < 0 + b < 0 + c < 0;
   switch(count){
       case 0:
           if(a < b){
               min = c < a ? c : a;
               max = c > b ? c : b;
           } else{
               min = c < b ? c : b;
               max = c > a ? c : a;
           }
           result = max - min > 32;
           break;
        case 1:
            if(a < 0) result = a > -256;
            else if(b < 0) result = b > -256;
            else if(c < 0) result = c > -256;
         case 2:
            if(a >= 0) result = 6 * (b + c) > -256;
            else if(b >= 0) result = 6 * (a + c) > -256;
            else if(c >= 0) result = 6 * (a + b) > -256;
       case 3:
           if(a < b){
                if(c < a){
                    modmin = abs(c);
                    sum2 = a + b;
                } else{
                    modmin = abs(a);
                    sum2 = b + c;
                }
           } else{
               if(c < b){
                    modmin = abs(c);
                    sum2 = a + b;
                } else{
                    modmin = abs(b);
                    sum2 = a + c;
                }
           }
           bla = modmin;
           while(!(bla % 2)){
                bla /= 2;
           }
           if(sum2 < -256 && !(bla - 1) && modmin < 256){
                result = 1;
           } else{
                result = (abs(sum2) - modmin < 16) || sum2 < 16;
           }
   }
   return result;
}
