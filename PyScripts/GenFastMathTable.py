#! /usr/bin/env python
#coding=utf-8
'''
Description: 生成部分耗时的计算函数的预查表
'''
import os,sys
import math

const_rate_count = 50
const_filename = "fastmathtables.h"


PI = 3.1415927;

SIN_BITS = 13;
SIN_MASK = ~(-1 << SIN_BITS);
SIN_COUNT = SIN_MASK + 1;
radFull = PI * 2;
degFull = 360.0;
radToIndex = SIN_COUNT / radFull;
degToIndex = SIN_COUNT / degFull;
radiansToDegrees = 180.0 / PI;
degreesToRadians = PI / 180.0;

sintab = []
costab = []

for i in range(0, SIN_COUNT):
        a = (float)((i + 0.5) / SIN_COUNT * radFull);
        sintab.append((float)(math.sin(a)))
        costab.append((float)(math.cos(a)))

for i in range(0, 360, 90):
	sintab[(int)(i * degToIndex) & SIN_MASK] = (float)(math.sin(i * degreesToRadians));
	costab[(int)(i * degToIndex) & SIN_MASK] = (float)(math.cos(i * degreesToRadians));
def array2str(ary):
    linestr = "    "
    for i in range(0, len(ary)):
            linestr = "%s %f," % (linestr, ary[i])
            if (i % 8 == 7 and i < SIN_COUNT-1):
                    linestr += "\n    "
    #去掉最后一个","
    linestr = linestr[0:-1] + "\n"
    return linestr

    
def main(args):
    fh = open(const_filename, "w")
    fh.writelines("#ifndef _fastmath_h_\n")
    fh.writelines("#define _fastmath_h_\n\n")

    fh.writelines("static const float PI = 3.1415927;\n")
    fh.writelines("static const int SIN_BITS = 13;\n")
    fh.writelines("static const int SIN_MASK = ~(-1 << SIN_BITS);\n")
    fh.writelines("static const int SIN_COUNT = SIN_MASK + 1;\n")
    fh.writelines("static const float radFull = PI * 2;\n")
    fh.writelines("static const float degFull = 360;\n")
    fh.writelines("static const float radToIndex = SIN_COUNT / radFull;\n")
    fh.writelines("static const float degToIndex = SIN_COUNT / degFull;\n")
    fh.writelines("static const float radiansToDegrees = 180f / PI;\n")
    fh.writelines("static const float degreesToRadians = PI / 180;\n")
    fh.writelines("\n")
    
    fh.writelines("static const float SINTables[SIN_COUNT] = {\n")
    fh.writelines(array2str(sintab))
    fh.writelines("};\n")
    fh.writelines("static const float COSTables[SIN_COUNT] = {\n")
    fh.writelines(array2str(costab))
    fh.writelines("};\n")
    fh.writelines("#endif //_fastmath_h_\n")

    print "finished!"

if __name__ == "__main__":
    main(sys.argv)
