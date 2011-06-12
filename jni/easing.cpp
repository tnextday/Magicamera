#include "easing.h"
#include <math.h>

double easeOutCubic( double t )
{
    t-=1.0;
    return t*t*t + 1.0;
}

double easeShake( double t, int round /*= 5*/ )
{
    // PI/2~PI*11/2
    return cos(M_PI_2 + round*M_PI*t)*(1.0f - t);
}

double easeInOutCirc(double t)
{
    t*=double(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    } else {
        t -= double(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
}