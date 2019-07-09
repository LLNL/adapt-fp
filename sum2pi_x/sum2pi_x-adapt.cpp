/**
 * sum2pi_x
 *
 * CRAFT demo app. Calculates pi*x in a computationally-heavy way that
 * demonstrates how to use CRAFT without being too complicated.
 *
 */

#include <stdio.h>
#include <math.h>
#include "adapt.h"
#include "adapt-impl.cpp"

/* macros */
#define ABS(x) ( ((x) < 0.0) ? (-(x)) : (x) )

/* constants */
#define PI     3.1415926535897932384626433832795
#define EPS    5e-7

/* loop  iterations; OUTER is X */
#define INNER    25
#define OUTER    2000

int main()
{
    AD_real sum = 0.0;
    AD_real tmp;
    AD_real acc;
    int i, j;

    AD_begin();
    AD_enable_absolute_value_error();   /* use absolute error because of repeated
                                           contributions from sum to itself */
    AD_INTERMEDIATE(sum, "sum");

    for (i=0; i<OUTER; i++) {
        acc = 0.0;
        AD_INTERMEDIATE(acc, "acc");
        for (j=1; j<INNER; j++) {

            /* accumulatively calculate pi */
            tmp = PI / pow(2.0, j);
            AD_INTERMEDIATE(tmp, "tmp");
            acc = acc + tmp;
            AD_INTERMEDIATE(acc, "acc");
        }
        sum = sum + acc;
        AD_INTERMEDIATE(sum, "sum");
    }

    AD_DEPENDENT(sum, "sum", EPS*(OUTER*PI));
    AD_report();

    double answer = (double)OUTER * PI;             /* correct answer */
    double diff = (double)answer-AD_value(sum);
    double error = ABS(diff);

    if ((double)error < (double)EPS*answer) {
        printf("SUM2PI_X - SUCCESSFUL!\n");
    } else {
        printf("SUM2PI_X - FAILED!!!\n");
    }

    return 0;
}

