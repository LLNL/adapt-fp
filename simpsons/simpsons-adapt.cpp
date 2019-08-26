#include <time.h>
#include <stdarg.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#include "adapt.h"
#include "adapt-impl.cpp"



AD_real pi;

AD_real fun(AD_real xarg) {
  AD_real result;
  result = sin(pi * xarg);                  AD_INTERMEDIATE(result, "result");
  return result;
}

int main( int argc, char **argv) {
  AD_begin();

  const int n = 1000000;
  AD_real a; AD_real b;
  AD_real h; AD_real s1; AD_real x;

  a = 0.0;                                  AD_INDEPENDENT(a, "a");
  b = 1.0;                                  AD_INDEPENDENT(b, "b");
  pi = acos(-1.0);                          AD_INDEPENDENT(pi, "pi");
  h = (b - a) / (2.0 * n);                  AD_INTERMEDIATE(h, "h");
  s1= 0.0;                                  AD_INTERMEDIATE(s1, "s1");

  x = a;                                    AD_INTERMEDIATE(x, "x");
  s1 = (fun(a) + fun(b));                   AD_INTERMEDIATE(s1, "s1");

  for(int l = 0; l < n; l++) { // ITERS before
    x += h;                                 AD_INTERMEDIATE(x, "x");
    s1 = s1 + 4.0 * fun(x);                 AD_INTERMEDIATE(s1, "s1");
    x = x + h;                              AD_INTERMEDIATE(x, "x");
    s1 = s1 + 2.0 * fun(x);                 AD_INTERMEDIATE(s1, "s1");
  }
  s1 = s1 * h * pi / 3.0;
  AD_DEPENDENT(s1, "s1", 1e-7);
  printf("ans: %.6e\n", AD_value(s1));
  AD_end();
  AD_report();

  return 0;
}

