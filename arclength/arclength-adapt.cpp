#include <stdio.h>
#include <math.h>
#include "adapt.h"
#include "adapt-impl.cpp"

/*
 * Precimonious output labels located in comments beside variable declarations
 */

#define ABS(x) ( ((x) < 0.0) ? (-(x)) : (x) )

#define PI  3.1415926535897932L
#define ANS 5.795776322412856L
#define EPS 1e-10

#define N 1000000

AD_real h  = PI / (double)N;     // double

AD_real t1 = 0.0;
AD_real t2;
AD_real t3;                      // double

AD_real s1 = 0.0;                // double

AD_real d1 = 1.0;
AD_real d2;                      // float

AD_real fun (AD_real x)
{
    d2 = d1;    // also d1 in original
    AD_INTERMEDIATE(d2, "d2");
    t3 = x;     // also t1 in original
    AD_INTERMEDIATE(t3, "t3");

    int k;
    for (k = 1; k <= 5; k+=1)
    {
        d2 = 2.0 * d2;
        AD_INTERMEDIATE(d2, "d2");
        t3 = t3 + sin (d2 * x) / d2;
        AD_INTERMEDIATE(t3, "t3");
    }
    return t3;
}

void do_fun ()
{
    int i;
    for (i = 1; i <= N; i+=1)
    {
        t2 = fun (i * h);
        AD_INTERMEDIATE(t2, "t2");
        s1 = s1 + sqrt (h * h + (t2 - t1) * (t2 - t1));
        AD_INTERMEDIATE(s1, "s1");
        t1 = t2;
        AD_INTERMEDIATE(t1, "t1");
    }
}

int main (int argc, char **argv)
{
    AD_begin();
    AD_INTERMEDIATE(h, "h");
    AD_INTERMEDIATE(t1, "t1");
    AD_INTERMEDIATE(s1, "s1");
    AD_INTERMEDIATE(d1, "d1");

    do_fun();

    AD_DEPENDENT(s1, "s1", EPS);
    AD_report();

    double error = ABS((double)ANS - AD_value(s1));
    if ((double)error < (double)EPS) {
        printf("Arclength - SUCCESSFUL!\n");
    }
    else {
        printf("Arclength - FAILED!\n");
    }

    return 0;
}
