#include "easing.h"
#include <math.h>

double easeOutCubic( double t )
{
	t-=1.0;
	return t*t*t + 1;
}

double easeShake( double t, int round /*= 5*/ )
{
	// PI/2~PI*11/2
	return cos(M_PI_2 + round*M_PI*t)*(1.0f - t);
}