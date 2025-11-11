# Android.mk - 构建 libmemory.so
# 作者: 阿夜

LOCAL_PATH := $(call my-dir)

# ============================
# 模块: libmemory.so
# ============================
include $(CLEAR_VARS)

LOCAL_MODULE := libmemory
LOCAL_SRC_FILES := ../c_driver.cpp

# C++ 标准和编译选项
LOCAL_CXXFLAGS := -std=c++17 -fPIC -Wall

# 头文件搜索路径
LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

# 链接库（如果需要）
# LOCAL_LDLIBS := -llog

# 构建共享库
include $(BUILD_SHARED_LIBRARY)