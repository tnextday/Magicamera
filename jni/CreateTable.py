#! /usr/bin/env python
#coding=utf-8

import os,sys
import math

const_rate_count = 50
const_filename = "ratetables.h"

def easeInOutCubic(t):
    t*=2.0
    if(t < 1.0):
        return 0.5*t*t*t;
    else:
        t -= 2.0;
        return 0.5*(t*t*t + 2.0);

def easeInOutQuad(t):
    t*=2.0;
    if (t < 1.0):
        return t*t/2.0;
    else :
        --t;
        return -0.5 * (t*(t-2.0) - 1.0);

def easeInOutSine(t):
    return -0.5 * (math.cos(math.pi*t) - 1.0);

    
def main(args):
    fh = open(const_filename, "w")
    fh.writelines("#ifndef _ratetables_h_\n")
    fh.writelines("#define _ratetables_h_\n\n")

    fh.writelines("//预先生成的形变衰减率\n")
    fh.writelines("static const int RateTablesCount = %d;\n" % const_rate_count)
    fh.writelines("static const float RateTables[RateTablesCount][RateTablesCount] = {\n")
    for y in range(0, const_rate_count):
        linestr = "    {"
        for x in range(0, const_rate_count):
            r = math.sqrt(math.pow(x, 2) + math.pow(y, 2))
            r = r/const_rate_count
            if (r > 1.0):
                r = 1.0
            #r = easeInOutSine(1.0 - r)*0.9
            r = (1.0 - r)*0.9
            if (x < const_rate_count - 1):
                linestr = "%s %0.4f," % (linestr, r)
            else:
                linestr = "%s %0.4f},\n" % (linestr, r)
                fh.writelines(linestr)
                
    fh.writelines("};\n")    
    fh.writelines("#endif _ratetables_h_\n")

    print "finished!"

if __name__ == "__main__":
    main(sys.argv)
