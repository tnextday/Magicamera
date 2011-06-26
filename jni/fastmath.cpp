#include "fastmath.h"


static const float radFull = PI * 2;
static const float degFull = 360.0;
static const float radToIndex = SIN_COUNT / radFull;
static const float degToIndex = SIN_COUNT / degFull;

float fastsin (float rad) {
    return SINTables[(int)(rad * radToIndex) & SIN_MASK];
}

float fastcos (float rad) {
    return COSTables[(int)(rad * radToIndex) & SIN_MASK];
}

float fastsinDeg (float deg) {
    return SINTables[(int)(deg * degToIndex) & SIN_MASK];
}

float fastcosDeg (float deg) {
    return COSTables[(int)(deg * degToIndex) & SIN_MASK];
}


