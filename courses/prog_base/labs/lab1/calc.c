#include <math.h>

double calc(double x, double y, double z) {
   double a;
   double a0, a1, a2;

   if(!((x - y) && z && (x + y) && sin(x))) return NAN;

   a0 = pow(x, y + 1)/pow(x - y, 1/z);
   a1 = y/(6 * fabs(x + y));
   a2 = sqrt(fabs(cos(y)/sin(x) + 1));

   a = a0 +  a1 + a2;
   return a;
}
