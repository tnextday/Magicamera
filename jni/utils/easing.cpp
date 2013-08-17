#include "easing.h"
#include <math.h>

inline float easeShake( double t, int round /*= 6*/ )
{
    return 1.0 - cos(M_PI_2 + round*M_PI*t)*(1.0f - t);
}

float easeOutBounce_helper( double t, double c, double a )
{
    if (t == 1.0) return c;
    if (t < (4/11.0)) {
        return c*(7.5625*t*t);
    } else if (t < (8/11.0)) {
        t -= (6/11.0);
        return -a * (1. - (7.5625*t*t + .75)) + c;
    } else if (t < (10/11.0)) {
        t -= (9/11.0);
        return -a * (1. - (7.5625*t*t + .9375)) + c;
    } else {
        t -= (21/22.0);
        return -a * (1. - (7.5625*t*t + .984375)) + c;
    }
}

float CEaseShake::update( double t )
{
    return easeShake(t, m_round);
}

CEaseShake::CEaseShake( int round /*= 6*/ )
{
    m_round = 6;
}


float CEaseOutCubic::update( double t )
{
    t-=1.0;
    return t*t*t + 1.0;
}

float CEaseInOutCirc::update( double t )
{
    t*=double(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    } else {
        t -= double(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
}


float CEaseOutBounce::update( double t )
{
    return easeOutBounce_helper(t, 1, m_amplitude);
}

CEaseOutBounce::CEaseOutBounce( double amplitude /*= 1.0*/ )
{
    m_amplitude = amplitude;
}

float CEaseInCubic::update( double t )
{
    return t*t*t;
}

float CEaseOutQuart::update( double t )
{
    t -= 1.0;
    return - (t*t*t*t- 1);
}

float CEaseInQuart::update( double t )
{
    return t*t*t*t;
}