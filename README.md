# libscan 内核读写动态库

这是一个示例项目，展示如何使用 C++ 编写自己的内核读写动态库。通过这个项目，你可以实现内核扫描功能。

## 项目结构

```
project/
├── CMakeLists.txt
├── c_driver.h
├── c_driver.cpp
└── README.md
```

## 编译动态库

### 安装依赖

确保你已经安装了 CMake 和 GCC 编译器。如果没有安装，可以使用以下命令安装（以 Ubuntu 为例）：

```sh
sudo apt update
sudo apt install cmake g++
```

### 编译步骤

1. 在项目根目录下创建一个 `build` 目录并进入该目录。
2. 运行 CMake 和 `make` 命令来编译动态库。

```sh
mkdir build
cd build
cmake ..
make
```

编译完成后，你将在 `build` 目录下找到生成的动态库文件 `libmemory.so`。

## 内核读写功能

### 功能概述

动态库 `libmemory.so` 提供了以下内核读写功能：

- **`fsinit_kernel`**：初始化内核环境。
- **`fsget_init`**：检查是否已初始化。
- **`fsinit_pid`**：初始化指定的进程 ID。
- **`fsread`**：从指定地址读取内存。


## 注意事项

- **动态库路径**：确保动态库路径正确。如果动态库不在标准路径中，可以使用绝对路径或相对路径加载动态库。
- **依赖库**：使用 `ldd` 命令检查动态库的依赖关系，确保所有依赖库都存在且路径正确。
- **权限**：确保动态库文件有可执行权限。如果需要，可以使用 `chmod +x libmemory.so` 命令设置权限。
- **运行时路径**：如果动态库在非标准路径下，可以设置 `LD_LIBRARY_PATH` 环境变量，让系统能够找到动态库：

  ```sh
  export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
  ```

## 联系方式

如果有任何问题或建议，请通过以下方式联系我们：

- **Email**：2997036064@qq.com
- **GitHub**：https://github.com/AYssu/fast-scan/releases

感谢您的支持和贡献！


