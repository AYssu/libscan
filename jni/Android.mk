# Android.mk - 构建 lib$(DRIVER_NAME).so
# 模块名可配置：默认 rthookpro，命令行 DRIVER_NAME=xxx 覆盖（如 DRIVER_NAME=foo -> libfoo.so）

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

DRIVER_NAME ?= rthookpro
LOCAL_MODULE := $(DRIVER_NAME)
LOCAL_SRC_FILES := ../c_driver.cpp

LOCAL_CXXFLAGS := -std=c++17 -fPIC -Wall

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_SHARED_LIBRARY)
