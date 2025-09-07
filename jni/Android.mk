# Android.mk for building a JNI library

# 设置模块名称
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# 设置模块名称
LOCAL_MODULE := libmemory

# 设置源文件
LOCAL_SRC_FILES := ../c_driver.cpp

# 设置 C++ 标准
LOCAL_CXXFLAGS := -std=c++17

# 设置包含目录
LOCAL_C_INCLUDES := $(LOCAL_PATH)

# 设置动态库的输出名称
LOCAL_LDLIBS := -llog -lc++_shared

# 构建共享库
include $(BUILD_SHARED_LIBRARY)