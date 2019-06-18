#include <stdio.h>
#include "adapt.h"
#include "adapt-impl.cpp"

AD_real p = 1.00000003;      // must be double
AD_real l = 0.00000003;      // can be float
AD_real o;

int main()
{
    AD_begin();
    AD_INTERMEDIATE(p, "p");
    AD_INTERMEDIATE(l, "l");
    o = p + l;
    AD_DEPENDENT(o, "o", 1e-8);
    AD_report();
    printf("%.8f\n", AD_value(o));    // should print 1.00000006
    return 0;
}
