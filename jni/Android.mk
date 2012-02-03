LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libcore
LOCAL_CFLAGS := -Werror

LOCAL_SRC_FILES :=	jni_main.cpp \
	actions/action.cpp \
	actions/moveto.cpp \
	actions/parallel.cpp \
	actions/rotateto2d.cpp \
	actions/sequence.cpp \
	effect/curveeffect.cpp \
	effect/effect.cpp \
	effect/effectfactory.cpp \
	effect/glow.cpp \
	effect/hdr.cpp \
	effect/infrared.cpp \
	effect/lemo.cpp \
	effect/microspur.cpp \
	effect/nostalgia.cpp \
	effect/overlay.cpp \
	effect/purple.cpp \
	effect/reversal.cpp \
	effect/scanline.cpp \
	effect/shift.cpp \
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
