# Android.mk for building a JNI library

# 设置模块名称
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# 设置模块名称
LOCAL_MODULE := libmemory

# 设置源文件
LOCAL_SRC_FILES := ../c_driver.cpp

# 设置 C++ 标准和静态链接 C++ 运行时库
LOCAL_CXXFLAGS := -std=c++17
APP_STL := c++_static

# 设置包含目录
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_LDLIBS += $(LOCAL_PATH)/../driver.a

# 清除之前的变量
include $(CLEAR_VARS)

# 定义模块名
LOCAL_MODULE := libkma

# 指定源文件
LOCAL_SRC_FILES := ../libkma.cpp

# 设置 C++ 标准
LOCAL_CXXFLAGS := -std=c++17

# 设置包含目录
LOCAL_C_INCLUDES := $(LOCAL_PATH)

# 添加静态库
LOCAL_LDLIBS += $(LOCAL_PATH)/../driver.a

# 构建共享库
include $(BUILD_SHARED_LIBRARY)

# 清除之前的变量
include $(CLEAR_VARS)

# 定义模块名
LOCAL_MODULE := libmemory

# 指定源文件
LOCAL_SRC_FILES := ../c_driver.cpp

# 设置 C++ 标准
LOCAL_CXXFLAGS := -std=c++17

# 设置包含目录
LOCAL_C_INCLUDES := $(LOCAL_PATH)

# 添加静态库
LOCAL_LDLIBS += $(LOCAL_PATH)/../driver.a

# 构建共享库
include $(BUILD_SHARED_LIBRARY)