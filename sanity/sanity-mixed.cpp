#include <stdio.h>

double p = 1.00000003;      // must be double
float l = 0.00000003;       // can be float
double o;

int main()
{
    o = p + l;
    printf("%.8f\n", (double)o);    // should print 1.00000006
    return 0;
}
