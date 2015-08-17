LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libSpeechProcessing
LOCAL_SRC_FILES := Timer.c SpeechProcessing.c Transforms.c MFCC.c GMM.c logMMSE.c
LOCAL_CFLAGS := -O3
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
