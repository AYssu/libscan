<div align="center">

# libscan

### 🚀 内核级内存读写动态库

一个功能强大的 Android 内存操作与网络隔离库

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Android-green.svg)](https://www.android.com)
[![Arch](https://img.shields.io/badge/arch-arm64--v8a-orange.svg)](https://developer.android.com/ndk/guides/abis)
[![NDK](https://img.shields.io/badge/NDK-r21e+-red.svg)](https://developer.android.com/ndk)
[![API](https://img.shields.io/badge/API-21%2B-brightgreen.svg)](https://android-arsenal.com/api?level=21)

[功能特性](#-功能特性) • [快速开始](#-快速开始) • [编译指南](#-编译指南) • [API 文档](#-api-文档) • [部署说明](#-部署说明)

</div>

---

## 📖 简介

libscan 是一个专为 Android 平台设计的内核级内存操作库，提供完整的内存读写、syscall 拦截等功能。采用模块化 Driver 架构，支持多种内存访问方式，适用于游戏辅助、性能分析、逆向工程等场景。

### 💡 核心优势

- 🔧 **Driver 架构** - 模块化设计，轻松切换不同的内存读写方式
- 🔄 **多平台支持** - 同时支持真机 (ARM64) 和模拟器 (x86_64)
- ⚡ **高性能** - 优化的内存访问，支持批量操作和指针链
- 🛠️ **易于集成** - GitHub Actions 自动化编译和发布
- 🎯 **双模式** - 提供直接调用和 syscall hook 两种使用方式

## ✨ 功能特性

### 📦 提供两个核心库

| 库文件 | 说明 | 特点 |
|--------|------|------|
| **libmemory.so** | 基础内核读写库 | 提供 C API 接口，直接调用内存读写功能 |
| **libkma.so** | syscall 拦截库 | 通过 hook `process_vm_readv/writev` 实现透明内存访问 |

### 🎯 主要功能

- ✅ 多种 Driver 支持（syscall、pread64、kernel 等）
- ✅ 指针链自动解析
- ✅ 模糊搜索与精确搜索
- ✅ 批量读写优化
- ✅ 类型安全的内存访问
- ✅ 自动构建与发布

## 📁 项目结构

```
libscan/
├── CMakeLists.txt          # CMake 构建配置
├── c_driver.h              # C API 头文件
├── c_driver.cpp            # libmemory.so 源码
├── libkma.cpp              # libkma.so 源码（syscall hook）
├── driver.h                # 内核驱动接口头文件
├── driver.a                # 内核驱动静态库
├── lib5.so                 # 辅助库
├── jni/                    # NDK 构建配置
│   ├── Android.mk          # NDK 构建脚本
│   └── Application.mk      # 目标平台配置
├── .github/workflows/      # GitHub Actions
│   └── auto-build.yml      # 自动构建发布
├── 一键构建脚本.sh          # NDK 自动构建脚本
└── README.md
```

---

## 🚀 快速开始

### 📥 下载预编译版本

前往 [Releases](https://github.com/AYssu/libscan/releases) 页面下载最新的 `libscan-arm64-v8a.zip`

```bash
# 解压文件
unzip libscan-arm64-v8a.zip

# 推送到 Android 设备
adb push libmemory.so /data/local/tmp/
adb shell chmod 755 /data/local/tmp/libmemory.so
```

### 💻 基本使用

```cpp
#include <dlfcn.h>

// 加载动态库
void* handle = dlopen("/data/local/tmp/libmemory.so", RTLD_NOW);

// 初始化目标进程
auto init_pid = (bool(*)(pid_t))dlsym(handle, "fsinit_pid");
init_pid(12345);

// 读取内存
auto read_mem = (bool(*)(uintptr_t, void*, size_t))dlsym(handle, "fsread");
int value;
read_mem(0x7000000000, &value, sizeof(value));
```

---

## 🔨 编译指南

项目支持两种编译方式：**CMake** 和 **NDK**

<details>
<summary><b>方式一：使用 CMake 编译（推荐）</b></summary>

### 1. 安装依赖

确保你已经安装了 CMake 和 GCC/Clang 编译器：

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake g++ make

# 如果需要交叉编译到 aarch64，还需要安装：
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```

**Android Termux:**
```bash
pkg install cmake clang make
```

### 2. 编译步骤

#### 本地架构编译

```bash
# 在项目根目录下创建构建目录
mkdir -p build
cd build

# 运行 CMake 配置
cmake ..

# 编译
make -j$(nproc)
```

#### 交叉编译到 ARM64/aarch64

```bash
# 创建构建目录
mkdir -p build-aarch64
cd build-aarch64

# 使用 aarch64 工具链编译
cmake .. \
  -DCMAKE_SYSTEM_NAME=Linux \
  -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
  -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
  -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
  -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
  -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
  -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY

# 编译
make -j$(nproc)
```

#### 使用 Android NDK 工具链

```bash
# 配置 NDK 路径
# 方法1: 临时设置（仅当前终端会话有效）
export NDK_PATH=/root/android-ndk-r21e

# 方法2: 永久设置（添加到 ~/.bashrc 或 ~/.zshrc）
echo 'export NDK_PATH=/root/android-ndk-r21e' >> ~/.bashrc
source ~/.bashrc

# 方法3: 使用默认路径（推荐）
# 项目默认 NDK 路径为 /root/android-ndk-r21e
export NDK_PATH=${NDK_PATH:-/root/android-ndk-r21e}

# 验证 NDK 路径是否正确
ls $NDK_PATH/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android*-clang

# 创建构建目录并编译
mkdir -p build-android
cd build-android

# 使用 NDK 工具链
cmake .. \
  -DCMAKE_SYSTEM_NAME=Android \
  -DCMAKE_SYSTEM_VERSION=21 \
  -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
  -DCMAKE_ANDROID_NDK=$NDK_PATH \
  -DCMAKE_C_COMPILER=$NDK_PATH/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang \
  -DCMAKE_CXX_COMPILER=$NDK_PATH/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang++

# 编译
make -j$(nproc)
```

### 3. 输出文件

编译完成后，动态库文件将位于：
```
build/libs/<架构>/libmemory.so
build/libs/<架构>/libkma.so
```

其中 `<架构>` 根据你的系统自动检测（如 `aarch64`、`x86_64` 等）。

</details>

<details>
<summary><b>方式二：使用 NDK 编译</b></summary>

### 1. 安装 Android NDK

下载并配置 Android NDK，推荐版本：r21e 或更高。

### 2. 使用一键脚本

```bash
# 执行自动构建脚本
bash 一键构建脚本.sh
```

脚本会自动查找 NDK 路径并编译。

### 3. 手动编译

```bash
# 直接运行 ndk-build
ndk-build

# 或指定 NDK 路径
/path/to/ndk/ndk-build
```

### 4. 输出文件

编译完成后，动态库文件将位于：
```
libs/arm64-v8a/libmemory.so
libs/arm64-v8a/libkma.so
```

</details>

---

## 📚 API 文档

### libmemory.so - 基础内核读写库

提供以下 C API 函数：

| 函数签名 | 功能说明 |
|----------|---------|
| `bool fsinit_kernel()` | 初始化内核驱动环境 |
| `bool fsget_init()` | 检查驱动是否已初始化 |
| `bool fsinit_pid(pid_t pid)` | 设置目标进程 PID |
| `bool fsread(uintptr_t addr, void *buffer, size_t size)` | 从目标进程读取内存 |

#### 📝 使用示例

```cpp
#include <dlfcn.h>
#include <cstdint>

// 加载动态库
void* handle = dlopen("/data/local/tmp/libmemory.so", RTLD_NOW);

// 获取函数指针
auto fsinit_pid = (bool(*)(pid_t))dlsym(handle, "fsinit_pid");
auto fsread = (bool(*)(uintptr_t, void*, size_t))dlsym(handle, "fsread");

// 使用函数
int target_pid = 12345;
fsinit_pid(target_pid);

char buffer[256];
uintptr_t addr = 0x7000000000;
fsread(addr, buffer, sizeof(buffer));
```

### libkma.so - syscall 拦截库

通过 LD_PRELOAD 或手动加载，自动拦截 `process_vm_readv` 和 `process_vm_writev` 系统调用，将其重定向到内核驱动。

#### 📝 使用方式

```bash
# 方法1: LD_PRELOAD 注入到目标程序
LD_PRELOAD=/data/local/tmp/libkma.so ./your_app

# 方法2: 配合 lib5.so 使用（详见项目源码）
```

---

## 📦 部署说明

### 方式一：Android 设备部署（标准）

**重要提示：** 在 ARM64 Android 设备上，动态库必须放置在 `/data/local/tmp/` 目录下。

```bash
# 1. 将编译好的库文件推送到设备
adb push build/libs/aarch64/libmemory.so /data/local/tmp/
adb push build/libs/aarch64/libkma.so /data/local/tmp/
adb push lib5.so /data/local/tmp/

# 2. 设置可执行权限
adb shell chmod 755 /data/local/tmp/lib*.so

# 3. 推送内核模块（如果需要）
adb push kma_v6.6.426.kpm /data/local/tmp/

# 4. 验证部署
adb shell ls -lh /data/local/tmp/lib*.so
```

### 方式二：配合 GameGuardian 部署

```bash
# 1. libmemory.so 放到系统临时目录
adb push libmemory.so /data/local/tmp/libmemory.so

# 2. 其他库放到 GG 内部目录
# 路径示例: /data/data/catch_.me_.if_.you_.can_/files/
adb push lib5.so /data/data/<gg_package>/files/
adb push lib5.so.primary /data/data/<gg_package>/files/
adb push libkma.so /data/data/<gg_package>/files/

# 3. 修改 GG 目录权限
adb shell chown -R <uid>:<uid> /data/data/<gg_package>/files/
```

---

## ⚠️ 注意事项

| 项目 | 说明 |
|------|------|
| **🏗️ 架构要求** | 仅支持 ARM64（aarch64）架构 |
| **🔐 权限要求** | 需要 root 权限才能正常使用 |
| **📍 路径限制** | libmemory.so 必须放在 `/data/local/tmp/` |
| **📱 系统版本** | 建议 Android 8.0+ (API 21+) |
| **🔧 依赖库** | 确保 driver.a 与目标架构匹配 |
| **💾 内核模块** | 需要先加载 kma_v*.kpm 内核模块 |

## 🔧 故障排查

| 问题 | 解决方案 |
|------|----------|
| 找不到动态库 | 确认文件在 `/data/local/tmp/` 目录下 |
| 权限被拒绝 | 使用 `chmod 755` 设置可执行权限 |
| 符号未定义 | 检查 `driver.a` 是否正确链接 |
| 读取失败 | 确认内核模块已加载且有 root 权限 |

---

## 🔗 相关资源

- 📘 **内核 GG 对接示例**: [Gg_Docking_Kernel](https://github.com/AYssu/Gg_Docking_Kernel)
- 🚀 **Fast-Scan 发布页**: [Releases](https://github.com/AYssu/fast-scan/releases)
- 📚 **开发文档**: 查看 [Wiki](https://github.com/AYssu/libscan/wiki)（即将推出）

## 📄 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

**⚠️ 免责声明**: 本项目仅供学习研究使用，请勿用于非法用途。使用本项目产生的任何后果由使用者自行承担。

## 📮 联系方式

<div align="center">

**遇到问题？欢迎联系我们！**

[![Email](https://img.shields.io/badge/Email-2997036064@qq.com-red?style=flat-square&logo=gmail)](mailto:2997036064@qq.com)
[![GitHub](https://img.shields.io/badge/GitHub-AYssu/libscan-black?style=flat-square&logo=github)](https://github.com/AYssu/libscan)
[![Issues](https://img.shields.io/github/issues/AYssu/libscan?style=flat-square)](https://github.com/AYssu/libscan/issues)

</div>

## 🙏 致谢

感谢所有为本项目做出贡献的开发者！

如果这个项目对你有帮助，请给个 ⭐ Star 支持一下！

---

<div align="center">

**作者：阿夜**

Made with ❤️ by [AYssu](https://github.com/AYssu)

</div>