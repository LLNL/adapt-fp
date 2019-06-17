#include <stdio.h>

float p = 1.00000003;      // must be double
float l = 0.00000003;      // can be float
float o;                   // can also be float (but not so interesting)

int main()
{
#   pragma adapt begin
    o = p + l;
#   pragma adapt output o 1e-8
#   pragma adapt end
    printf("%.8g\n", (double)o);    // should print 1.0000001
    return 0;
}
