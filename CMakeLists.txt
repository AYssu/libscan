cmake_minimum_required(VERSION 3.10)
project(libscan)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 检测目标架构
if(NOT CMAKE_SYSTEM_PROCESSOR)
    set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# 设置库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${CMAKE_SYSTEM_PROCESSOR})

# 添加头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR})

# 导入静态库 driver.a
add_library(driver STATIC IMPORTED)
set_target_properties(driver PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/driver.a
)

# 构建 libmemory.so - 基础内核读写库
add_library(memory SHARED c_driver.cpp)
target_link_libraries(memory PRIVATE driver)
set_target_properties(memory PROPERTIES OUTPUT_NAME "memory")

# 构建 libkma.so - syscall拦截库
add_library(kma SHARED libkma.cpp)
target_link_libraries(kma PRIVATE driver)
set_target_properties(kma PROPERTIES OUTPUT_NAME "kma")

# 安装规则
install(TARGETS memory kma
    LIBRARY DESTINATION lib
)

# 打印构建信息
message(STATUS "Building for architecture: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# 作者：阿夜
# 项目地址：https://github.com/AYssu/libscan