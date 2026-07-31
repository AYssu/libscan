# Android.mk - 构建 libsyscall.so

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libsyscall
LOCAL_SRC_FILES := ../c_driver.cpp

LOCAL_CXXFLAGS := -std=c++17 -fPIC -Wall

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_SHARED_LIBRARY)