# Application.mk - NDK 构建全局配置
# 作者: 阿夜

# 目标架构
APP_ABI := arm64-v8a

# 最低 Android API 版本
APP_PLATFORM := android-21

# C++ 标准库（静态链接）
APP_STL := c++_static

# 优化级别（可选）
# APP_OPTIM := release