#! /usr/bin/env python
#coding=utf-8
'''
Description: 根据相应的图片生成相应xml drawable文件
'''

import os
from os import path
import glob


g_res_dir = path.join('..','res')

g_nor_endswith = '_nor'
g_press_endswith = '_press'
g_disable_endswith = '_disable'

g_image_drawable_dir = path.join(g_res_dir,'drawable-hdpi')
g_xml_drawable_dir = path.join(g_res_dir,'drawable')

def writeDrawableXML(name):
    xmlPath = path.join(g_xml_drawable_dir, name + '.xml')
    fh = open(xmlPath, 'w')
    fh.writelines('<?xml version="1.0" encoding="utf-8"?>\n')
    fh.writelines('<selector xmlns:android="http://schemas.android.com/apk/res/android">\n')
    filePath = path.join(g_image_drawable_dir, name);
    pressFile = filePath+g_press_endswith
    if (path.exists(pressFile+'.png') or path.exists(pressFile+'.9.png')):
        fh.writelines('    <item android:state_pressed="true" android:drawable="@drawable/%s"/>\n'%(name+g_press_endswith))
    disableFile = filePath+g_disable_endswith
    if (path.exists(disableFile+'.png') or path.exists(disableFile+'.9.png')):
        fh.writelines('    <item android:state_enabled="false" android:drawable="@drawable/%s"/>\n'%(name+g_disable_endswith))
    fh.writelines('    <item android:drawable="@drawable/%s"/>\n'%(name+g_nor_endswith))
    fh.writelines('</selector>')
    print xmlPath,'has write.'



def main():
    images = glob.glob(path.join(g_image_drawable_dir, '*.png'))
    for image in images:
        imageName = path.basename(image).split('.')[0]
        if imageName.endswith(g_nor_endswith):
            realName = imageName[0:-len(g_nor_endswith)]
            if (not path.exists(path.join(g_xml_drawable_dir, realName + '.xml'))):
                writeDrawableXML(realName)
    print 'finished.'

if __name__ == "__main__":
    main();
