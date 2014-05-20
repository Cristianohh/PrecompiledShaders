TOP_PATH := $(call my-dir)/..

LOCAL_PATH := $(TOP_PATH)

include $(CLEAR_VARS)

MY_LOCAL_C_INCLUDES := $(TOP_PATH)/../../src
LOCAL_C_INCLUDES := $(MY_LOCAL_C_INCLUDES)
LOCAL_C_INCLUDES += $(TOP_PATH)/../../../..

# automatically compile all .cpp and .c files
LOCAL_SRC_FILES := $(wildcard jni/*.cpp)
LOCAL_SRC_FILES += $(wildcard jni/*.c)
LOCAL_SRC_FILES += $(wildcard $(MY_LOCAL_C_INCLUDES)/*.cpp)
LOCAL_MODULE := precompiledapp
LOCAL_CFLAGS :=  -DANDROID 
LOCAL_LDLIBS := -llog -landroid -lGLESv3

include $(BUILD_SHARED_LIBRARY)
