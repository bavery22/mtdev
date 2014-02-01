LOCAL_PATH := $(call my-dir)

# Build libmtdev
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	src/caps.c \
	src/core.c \
	src/iobuf.c \
	src/match.c \
	src/match_four.c
#LOCAL_CFLAGS := -DSASR
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_MODULE := libmtdev
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)

# Build mtdev-test
include $(CLEAR_VARS)
LOCAL_SRC_FILES := test/mtdev-test.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := libmtdev
LOCAL_MODULE := mtdev-test
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)
