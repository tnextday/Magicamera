LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libmagic
LOCAL_CFLAGS := -Werror

LOCAL_SRC_FILES :=	jni_main.cpp \
	actions/action.cpp \
	actions/moveto.cpp \
	actions/parallel.cpp \
	actions/rotateto2d.cpp \
	actions/sequence.cpp \
	effect/imageeffect.cpp \
	gdx2d/gdx2d.cpp \
	gdx2d/stb_image.c \
	glutils/baseshader.cpp \
	glutils/framebufferobject.cpp \
	glutils/glutils.cpp \
	glutils/glyuvtexture.cpp \
	glutils/mesh.cpp \
	glutils/sprite.cpp \
	glutils/texture.cpp \
	glutils/textureregion.cpp \
	magic/effectengine.cpp \
	magic/imageadjust.cpp \
	magic/magicengine.cpp \
	magic/magicmain.cpp \
	magic/meshengine.cpp \
	ui/button.cpp \
	utils/arraylist.cpp \
	utils/easing.cpp \
	utils/fastmath.cpp \
	utils/fileutils.cpp \
	utils/mathelpers.cpp \
	utils/packageloader.cpp \
	zip/ioapi.cpp \
	zip/unzip.cpp \

LOCAL_LDLIBS    := -llog -lGLESv2 -lz 
include $(BUILD_SHARED_LIBRARY)
