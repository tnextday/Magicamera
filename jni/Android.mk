LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libmagicjni
LOCAL_CFLAGS    := -Werror

LOCAL_SRC_FILES := gl_code.cpp \
			glHelpers.cpp \
			glutils.cpp \
			mesh.cpp \
			texture.cpp

include $(LOCAL_PATH)/gdx2d/Android.mk

LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)
