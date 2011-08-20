#include "easing.h"
#include <math.h>

float easeShake( float t, int round /*= 6*/ )
{
    return cos(M_PI_2 + round*M_PI*t)*(1.0f - t);
}

float CEaseShake::update( float t )
{
    return easeShake(t, m_round);
}

CEaseShake::CEaseShake( int round /*= 6*/ )
{
    m_round = 6;
}


float CEaseOutCubic::update( float t )
{
    t-=1.0;
    return t*t*t + 1.0;
}

float CEaseInOutCirc::update( float t )
{
    t*=double(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    } else {
        t -= double(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
}

