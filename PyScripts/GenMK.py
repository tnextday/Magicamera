#! /usr/bin/env python
#coding=utf-8
'''
Description:自动生成Android.mk文件
'''
import os
import sys
from os import path

g_module_name = "libcore"
g_add_header_dir = ['facedetector']
g_src_dirs = []

g_jni_dir = os.path.join('..', 'jni')+path.sep
g_exts = [".cpp", ".c"]

def getfiles(dir, exts):
    result = []
    for parent, dirnames, filenames in os.walk(dir):
        for filename in filenames:
            if (path.splitext(filename)[1] in exts):
                basedir = parent.replace(g_jni_dir, '').replace(path.sep, '/')
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
    #fh.writelines("cmd-strip = $(TOOLCHAIN_PREFIX)strip --strip-debug -x $1\n")
    fh.writelines("include $(CLEAR_VARS)\n")
    fh.writelines("LOCAL_MODULE := "+g_module_name+"\n")
    #fh.writelines("LOCAL_CFLAGS := -Werror -fvisibility=hidden\n")
    fh.writelines("LOCAL_CFLAGS := -Werror\n")
    fh.writelines("\n")
    fh.writelines("LOCAL_SRC_FILES :=")
    #源码文件的顺序有要求，这个要怎么办呢，手动写目录？
    for src in srcs:
        fh.writelines("\t"+src+" \\\n")
    fh.writelines("\n")
    if (len(g_add_header_dir) > 0):
        fh.writelines("LOCAL_C_INCLUDES += \\\n")
        for hp in g_add_header_dir:
            fh.writelines("\t$(LOCAL_PATH)/"+hp+" \n")
    fh.writelines("LOCAL_LDLIBS    := -llog -lGLESv2 -lz \n")
    fh.writelines("include $(BUILD_SHARED_LIBRARY)\n")
    fh.close()

def main(args):
    cpps = getfiles(g_jni_dir, g_exts)
    writeMK(g_jni_dir, cpps)

if __name__ == "__main__":
    main(sys.argv)
