#include <stdio.h>

double p = 1.00000003;      // must be double
double l = 0.00000003;      // can be float
double o;

int main()
{
#   pragma adapt begin
    o = p + l;
#   pragma adapt output o 1e-8
#   pragma adapt end
    printf("%.8f\n", (double)o);    // should print 1.00000006
    return 0;
}
