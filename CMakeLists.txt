cmake_minimum_required(VERSION 3.10)
project(fscan)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加动态库
add_library(memory SHARED c_driver.cpp)

# 设置动态库的输出名称
set_target_properties(memory PROPERTIES OUTPUT_NAME "memory")

# 阿夜