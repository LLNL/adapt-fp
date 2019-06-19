#include <stdio.h>
#include <math.h>

/*
 * Precimonious output labels located in comments beside variable declarations
 */

#define ABS(x) ( ((x) < 0.0) ? (-(x)) : (x) )

#define PI  3.1415926535897932L
#define ANS 5.795776322412856L
#define EPS 1e-10

#define N 1000000

double h  = PI / (double)N;     // double

double t1 = 0.0;
double t2;
double t3;                      // double

double s1 = 0.0;                // double

float d1 = 1.0;
float d2;                       // float

double fun (double x)
{
    d2 = d1;    // also d1 in original
    t3 = x;     // also t1 in original

    int k;
    for (k = 1; k <= 5; k+=1)
    {
        d2 = 2.0 * d2;
        t3 = t3 + sin (d2 * x) / d2;
    }
    return t3;
}

void do_fun ()
{
    int i;
    for (i = 1; i <= N; i+=1)
    {
        t2 = fun (i * h);
        s1 = s1 + sqrt (h * h + (t2 - t1) * (t2 - t1));
        t1 = t2;
    }
}

int main (int argc, char **argv)
{
    do_fun();

    double error = ABS((double)ANS - (double)s1);
    if ((double)error < (double)EPS) {
        printf("Arclength - SUCCESSFUL!\n");
    }
    else {
        printf("Arclength - FAILED!\n");
    }

    return 0;
}
