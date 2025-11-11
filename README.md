# libscan - 内核读写动态库

这是一个基于内核驱动的内存读写动态库项目，支持 Android ARM64 平台。通过这个项目，你可以实现内核级别的进程内存扫描和读写功能。

## 项目简介

libscan 提供两种动态库：

- **libmemory.so** - 基础内核读写库，提供直接的内存读写接口
- **libkma.so** - syscall 拦截库，通过 hook `process_vm_readv`/`process_vm_writev` 系统调用实现透明的内核读写

## 项目结构

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
│   ├── Android.mk
│   └── Application.mk
├── 一键构建脚本.sh          # NDK 自动构建脚本
└── README.md
```

## 编译方式

项目支持两种编译方式：**CMake** 和 **NDK**

### 方式一：使用 CMake 编译（推荐）

#### 1. 安装依赖

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

#### 2. 编译步骤

**本地架构编译：**

```bash
# 在项目根目录下创建构建目录
mkdir -p build
cd build

# 运行 CMake 配置
cmake ..

# 编译
make -j$(nproc)
```

**交叉编译到 ARM64/aarch64（推荐用于 Android 设备）：**

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

**使用 Android NDK 工具链：**

```bash
# 1. 配置 NDK 路径
# 方法1: 临时设置（仅当前终端会话有效）
export NDK_PATH=/root/android-ndk-r21e

# 方法2: 永久设置（添加到 ~/.bashrc 或 ~/.zshrc）
echo 'export NDK_PATH=/root/android-ndk-r21e' >> ~/.bashrc
source ~/.bashrc

# 方法3: 使用默认路径（推荐）
# 项目默认 NDK 路径为 /root/android-ndk-r21e
export NDK_PATH=${NDK_PATH:-/root/android-ndk-r21e}

# 2. 验证 NDK 路径是否正确
ls $NDK_PATH/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android*-clang

# 3. 创建构建目录并编译
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

#### 3. 输出文件

编译完成后，动态库文件将位于：
```
build/libs/<架构>/libmemory.so
build/libs/<架构>/libkma.so
```

其中 `<架构>` 根据你的系统自动检测（如 `aarch64`、`x86_64` 等）。

### 方式二：使用 NDK 编译

#### 1. 安装 Android NDK

下载并配置 Android NDK，推荐版本：r21e 或更高。

#### 2. 使用一键脚本

```bash
# 执行自动构建脚本
bash 一键构建脚本.sh
```

脚本会自动查找 NDK 路径并编译。

#### 3. 手动编译

```bash
# 直接运行 ndk-build
ndk-build

# 或指定 NDK 路径
/path/to/ndk/ndk-build
```

#### 4. 输出文件

编译完成后，动态库文件将位于：
```
libs/arm64-v8a/libmemory.so
libs/arm64-v8a/libkma.so
```

## 库功能说明

### libmemory.so - 基础内核读写库

提供以下 C API 函数：

| 函数 | 功能说明 |
|------|---------|
| `bool fsinit_kernel()` | 初始化内核驱动环境 |
| `bool fsget_init()` | 检查驱动是否已初始化 |
| `bool fsinit_pid(pid_t pid)` | 设置目标进程 PID |
| `bool fsread(uintptr_t addr, void *buffer, size_t size)` | 从目标进程读取内存 |

**使用示例：**

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

通过 LD_PRELOAD 或手动加载，自动拦截 `process_vm_readv` 和 `process_vm_writev` 系统调用，将其重定向到内核驱动。使用方式：

```bash
# 方法1: LD_PRELOAD 注入到目标程序
LD_PRELOAD=/data/local/tmp/libkma.so ./your_app

# 方法2: 配合 lib5.so 使用（详见项目源码）
```

## 部署说明

### Android 设备部署

**重要提示：** 在 ARM64 Android 设备上，动态库必须放置在 `/data/local/tmp/` 目录下。

```bash
# 1. 将编译好的库文件推送到设备
adb push build/libs/aarch64/libmemory.so /data/local/tmp/
adb push build/libs/aarch64/libkma.so /data/local/tmp/
adb push lib5.so /data/local/tmp/

# 2. 设置可执行权限
adb shell chmod 755 /data/local/tmp/libmemory.so
adb shell chmod 755 /data/local/tmp/libkma.so
adb shell chmod 755 /data/local/tmp/lib5.so

# 3. 推送内核模块（如果需要）
adb push kma_v6.6.426.kpm /data/local/tmp/
```

### 验证部署

```bash
# 检查文件是否存在
adb shell ls -lh /data/local/tmp/lib*.so

# 检查依赖关系
adb shell "cd /data/local/tmp && ldd libmemory.so"
```

## 注意事项

1. **架构要求**：目前仅支持 ARM64（aarch64）架构
2. **内核模块**：需要先加载内核驱动模块 `kma_v6.6.426.kpm`
3. **Root 权限**：部分功能需要 root 权限
4. **路径限制**：在 Android ARM64 设备上，动态库路径硬编码为 `/data/local/tmp/`
5. **依赖库**：确保 `driver.a` 静态库与你的目标架构匹配

## 故障排查

| 问题 | 解决方案 |
|------|----------|
| 找不到动态库 | 确认文件在 `/data/local/tmp/` 目录下 |
| 权限被拒绝 | 使用 `chmod 755` 设置可执行权限 |
| 符号未定义 | 检查 `driver.a` 是否正确链接 |
| 读取失败 | 确认内核模块已加载且有 root 权限 |

## 相关资源

- **内核GG对接示例**：https://github.com/AYssu/Gg_Docking_Kernel.git
- **Fast-Scan 发布页**：https://github.com/AYssu/fast-scan/releases

## 许可证

本项目仅供学习研究使用，请勿用于非法用途。

## 联系方式

- **Email**：2997036064@qq.com
- **项目主页**：https://github.com/AYssu/libscan

---

**作者：阿夜**  
感谢您的支持和贡献！