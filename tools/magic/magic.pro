#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T23:31:12
#
#-------------------------------------------------

QT       += core gui

TARGET = magic
TEMPLATE = app

include(libcore.pri)
SOURCES += main.cpp\
    winmagic.cpp \
    render.cpp \
    eglhelper.cpp

HEADERS  += \
    winmagic.h \
    render.h \
    eglhelper.h

FORMS += \
    winmagic.ui

RESOURCES += \
    winmagic.qrc



win32: LIBS += -L$$PWD/../lib/ -llibEGL
win32: LIBS += -L$$PWD/../lib/ -lzlib
win32: LIBS += -L$$PWD/../lib/ -llibGLESv2

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

win32: PRE_TARGETDEPS += $$PWD/../lib/libGLESv2.lib
win32: PRE_TARGETDEPS += $$PWD/../lib/libEGL.lib
win32: PRE_TARGETDEPS += $$PWD/../lib/zlib.lib
