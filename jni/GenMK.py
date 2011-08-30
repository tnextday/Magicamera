#! /usr/bin/env python
#coding=utf-8

import os
import sys
from os import path

g_module_name = "libmagic"

g_exts = [".cpp", ".c"]

def getfiles(dir, exts):
    result = []
    for parent, dirnames, filenames in os.walk(dir):
        for filename in filenames:
            for ext in exts:
                if (filename.endswith(ext)):
                    basedir = os.path.basename(parent)
                    if (basedir == ""):
                        result += [ filename]
                    else:
                        result += [ basedir + "/" + filename]
    return result

def writeMK(dir, srcs):
    fName = path.join(dir, "Android.mk")
    fh = open(fName, "w")
    fh.seek(0, 2)
    fh.writelines("LOCAL_PATH:= $(call my-dir)\n")
    fh.writelines("include $(CLEAR_VARS)\n")
    fh.writelines("LOCAL_MODULE := "+g_module_name+"\n")
    fh.writelines("LOCAL_CFLAGS := -Werror\n")
    fh.writelines("\n")
    fh.writelines("LOCAL_SRC_FILES :=")
    for src in srcs:
        fh.writelines("\t"+src+" \\\n")
    fh.writelines("\n")
    fh.writelines("LOCAL_LDLIBS    := -llog -lGLESv2 -lz \n")
    fh.writelines("include $(BUILD_SHARED_LIBRARY)\n")
    fh.close()

def main(args):
    cpps = getfiles("./", g_exts)
    writeMK("./", cpps)

if __name__ == "__main__":
    main(sys.argv)
