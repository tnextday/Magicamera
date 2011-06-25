LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libmagicjni
LOCAL_CFLAGS    := -Werror

LOCAL_SRC_FILES := jni_main.cpp \
		magicengine.cpp \
		meshengine.cpp \
		easing.cpp \
		glHelpers.cpp \
		baseshader.cpp \
		framebufferobject.cpp \
		glutils.cpp \
		glyuvtexture.cpp \
		mesh.cpp \
		sprite.cpp \
		texture.cpp \
		textureregion.cpp

include $(LOCAL_PATH)/gdx2d/Android.mk

LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)
