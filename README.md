<div align="center">

# libscan

### 🚀 FastScan 内核读写替换库

专为 FastScan 设计的高性能内核级内存读写动态库

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Android-green.svg)](https://www.android.com)
[![Arch](https://img.shields.io/badge/arch-arm64--v8a-orange.svg)](https://developer.android.com/ndk/guides/abis)
[![NDK](https://img.shields.io/badge/NDK-r27c-red.svg)](https://developer.android.com/ndk)
[![FastScan](https://img.shields.io/badge/FastScan-Compatible-brightgreen.svg)](https://github.com/AYssu/fast-scan)

[功能特性](#-功能特性) • [快速开始](#-快速开始) • [编译指南](#-编译指南) • [API 文档](#-api-文档) • [部署说明](#-部署说明)

</div>

---

## 📖 简介

**libscan** 是一个专为 [FastScan](https://github.com/AYssu/fast-scan) 打造的内核级内存读写替换库。通过提供标准化的 C API 接口，你可以轻松替换 FastScan 中的内核读写实现，实现更强大的内存操作能力。

> ⚠️ **重要说明**：本项目与之前的 KMA 项目不同，不再提供 syscall hook 功能，专注于提供高性能的动态库替换方案。

### 💡 核心特点

- 🎯 **FastScan 专用** - 完全兼容 FastScan 的内核读写接口
- 🔧 **动态库替换** - 无需修改 FastScan，直接替换 libmemory.so
- ⚡ **高性能驱动** - 基于 rt_hookpro_driver 的内核级读写
- 🛠️ **简单易用** - 标准化 C API，易于集成和使用
- 🚀 **自动构建** - GitHub Actions 自动化编译发布

## ✨ 功能特性

### 📦 核心库说明

| 库文件 | 说明 | 用途 |
|--------|------|------|
| **libmemory.so** | FastScan 内核读写替换库 | 直接替换 FastScan 中的 libmemory.so 文件 |

### 🎯 主要功能

- ✅ **标准 C API** - 提供 `fsinit_kernel`、`fsget_init`、`fsinit_pid`、`fsread` 等接口
- ✅ **内核级读写** - 基于 rt_hookpro_driver 实现高性能内存访问
- ✅ **即插即用** - 编译后直接替换，无需修改 FastScan 代码
- ✅ **自动化发布** - 每次版本发布自动构建 ARM64 动态库
- ✅ **稳定可靠** - 经过充分测试，稳定支持 Android 8.0+

## 📁 项目结构

```
libscan/
├── c_driver.h              # C API 头文件（FastScan 兼容接口）
├── c_driver.cpp            # libmemory.so 核心实现
├── driver_rt_hookpro.h     # rt_hookpro_driver 驱动头文件
├── jni/                    # NDK 构建配置
│   ├── Android.mk          # NDK 构建脚本
│   └── Application.mk      # 目标平台配置
├── .github/workflows/      # GitHub Actions 自动化
│   └── auto-build.yml      # 自动构建和发布
├── 一键构建脚本.sh          # NDK 快速构建脚本
└── README.md
```

---

## 🚀 快速开始

### 📥 方式一：下载预编译版本（推荐）

1. 前往 [Releases](https://github.com/AYssu/libscan/releases) 页面下载最新的 `libscan-arm64-v8a.zip`

2. 解压并替换 FastScan 的 libmemory.so：

```bash
# 解压文件
unzip libscan-arm64-v8a.zip

# 推送到 FastScan 的库目录（通常是 /data/local/tmp/）
adb push libmemory.so /data/local/tmp/libmemory.so
adb shell chmod 755 /data/local/tmp/libmemory.so
```

3. 重启 FastScan，新的内核读写库将自动生效！

### 💻 方式二：在 FastScan 中使用

FastScan 会自动加载 `/data/local/tmp/libmemory.so`，无需修改任何代码。本库提供的接口与 FastScan 完全兼容：

```cpp
// FastScan 内部会自动调用这些接口
bool fsinit_kernel();              // 初始化内核环境
bool fsget_init();                 // 检查初始化状态
bool fsinit_pid(pid_t pid);        // 设置目标进程 PID
bool fsread(uintptr_t addr, void *buffer, size_t size);  // 读取内存
```

> 💡 **提示**：只需替换 libmemory.so 文件，FastScan 会自动使用新的内核读写实现。

---

## 🔨 编译指南

### 📋 前置要求

- Android NDK r21e 或更高版本
- 支持的系统：Linux、macOS、Windows (WSL)

<details>
<summary><b>方式一：使用 NDK 编译（推荐）</b></summary>

### 1. 安装 Android NDK

下载并配置 Android NDK，推荐版本：r21e 或更高。

### 2. 快速编译（推荐）

```bash
# 方式 A: 使用一键脚本
bash 一键构建脚本.sh

# 方式 B: 直接运行 ndk-build
ndk-build
```

脚本会自动查找以下 NDK 路径：
- `/data/user/0/aidepro.top/no_backup/ndksupport-1710240003/android-ndk-aide/ndk-build`
- `/data/data/com.termux/files/home/android-ndk-r21e/ndk-build`
- `/root/android-ndk-r23c/ndk-build`

### 3. 输出文件

编译完成后，动态库将位于：
```
libs/arm64-v8a/libmemory.so  ← 这就是要替换到 FastScan 的文件
```

### 4. 验证编译

```bash
# 检查生成的库文件
file libs/arm64-v8a/libmemory.so

# 输出应类似：
# libs/arm64-v8a/libmemory.so: ELF 64-bit LSB shared object, ARM aarch64
```

</details>

<details>
<summary><b>方式二：手动配置 NDK 编译</b></summary>

如果你的 NDK 在其他位置，可以手动指定路径：

```bash
# 设置 NDK 路径
export NDK_PATH=/path/to/your/android-ndk

# 运行编译
$NDK_PATH/ndk-build
```

或者修改 `一键构建脚本.sh`，添加你的 NDK 路径到 `ndk_paths` 数组中。

</details>

---

## 📚 API 文档

### FastScan 兼容接口

本库完全兼容 FastScan 的内核读写接口，提供以下 C API：

| 函数签名 | 功能说明 | FastScan 调用时机 |
|----------|---------|------------------|
| `bool fsinit_kernel()` | 初始化内核驱动环境 | FastScan 启动时调用 |
| `bool fsget_init()` | 检查驱动是否已初始化 | 内部状态检查 |
| `bool fsinit_pid(pid_t pid)` | 设置目标进程 PID | 选择进程后调用 |
| `bool fsread(uintptr_t addr, void *buffer, size_t size)` | 读取目标进程内存 | 搜索和修改时调用 |

### 实现说明

```cpp
// c_driver.cpp - 核心实现
#include "driver_rt_hookpro.h"

// 使用 rt_hookpro_driver 作为底层驱动
auto *driver = new rt_hookpro_driver();

extern "C" {
    bool fsinit_pid(pid_t pid) {
        driver->set_pid(pid);  // 设置目标进程
        return true;
    }
    
    bool fsread(uintptr_t addr, void *buffer, size_t size) {
        return driver->read(addr, buffer, size);  // 内核级读取
    }
}
```

> 💡 **无需修改代码**：FastScan 会自动调用这些接口，你只需替换 libmemory.so 文件即可。

---

## 📦 部署说明

### 替换 FastScan 的 libmemory.so

**核心步骤：** 只需将编译好的 libmemory.so 推送到 `/data/local/tmp/` 目录即可。

```bash
# 方式一：使用预编译版本（推荐）
# 从 Releases 下载后解压，然后执行：
adb push libmemory.so /data/local/tmp/libmemory.so

# 方式二：使用自己编译的版本
adb push libs/arm64-v8a/libmemory.so /data/local/tmp/libmemory.so

# 重启 FastScan 即可生效！
```

### 验证部署

```bash
# 检查文件是否存在
adb shell ls -lh /data/local/tmp/libmemory.so

# 输出应类似：
# -rwxr-xr-x 1 root root 123K 2025-11-11 22:00 /data/local/tmp/libmemory.so
```

---

## ⚠️ 注意事项

| 项目 | 说明 |
|------|------|
| **🎯 专用性** | 本库仅为 FastScan 设计，不是通用内存读写库 |
| **🏗️ 架构要求** | 仅支持 ARM64（aarch64）架构 |
| **🔐 权限要求** | 需要 root 权限才能正常使用 |
| **📍 路径要求** | libmemory.so 必须放在 `/data/local/tmp/` 目录 |
| **📱 系统版本** | 建议 Android 8.0+ (API 21+) |
| **🔄 兼容性** | 与 FastScan 完全兼容，无需修改 FastScan |

## 🔧 故障排查

| 问题 | 解决方案 |
|------|----------|
| **FastScan 找不到库** | 确认 libmemory.so 在 `/data/local/tmp/` 目录 |
| **FastScan 闪退** | 检查是否有 root 权限，尝试使用 Magisk 或 KernelSU |
| **读取失败** | 确认设备已 root，检查 SELinux 状态（可能需要 permissive） |
| **编译错误** | 确认 NDK 版本 >= r21e，检查 driver_rt_hookpro.h 是否存在 |

---

## 🔗 相关资源

- 🚀 **FastScan 项目**: [AYssu/fast-scan](https://github.com/AYssu/fast-scan)
- 📦 **FastScan 发布页**: [Releases](https://github.com/AYssu/fast-scan/releases)
- 📚 **本项目文档**: [Wiki](https://github.com/AYssu/libscan/wiki)（即将推出）
- 💬 **问题反馈**: [Issues](https://github.com/AYssu/libscan/issues)

## 📄 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

**⚠️ 免责声明**: 
- 本项目仅供学习研究使用，请勿用于非法用途
- 本库专为 FastScan 设计，不保证与其他工具的兼容性
- 使用本项目产生的任何后果由使用者自行承担

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