# libsyscall

基于 syscall 的内存读写插件，供 fscan 的 `--reader` 插件系统加载。

## 简介

libsyscall 编译为 `libsyscall.so`，通过 `dlopen` 被 fscan 主程序加载。导出 `create_memory_reader()` 接口，返回一个 `MemoryReader` 实例，主项目通过该实例读写目标进程内存。

底层使用 `process_vm_readv` / `process_vm_writev` syscall，无需内核模块。

## 导出接口

```cpp
extern "C" memtool::MemoryReader *create_memory_reader();
```

返回的实例实现以下方法：

| 方法 | 说明 |
|------|------|
| `init(pid_t)` | 设置目标进程 PID |
| `read(addr, buf, size)` | 读取目标进程内存 |
| `write(addr, buf, size)` | 写入目标进程内存 |
| `is_page_resident(addr)` | 页面驻留检测（插件内部处理，返回 false） |
| `check_pages(addr, size)` | 批量页面驻留检测（返回 nullptr） |
| `close()` | 释放资源 |
| `name()` | 返回读取器名称 |

## 文件结构

```
libscan/
├── driver_base.h           # 驱动基类接口
├── driver_syscall.h        # syscall 实现（process_vm_readv/writev）
├── memory_reader_impl.h    # MemoryReader 兼容接口定义
├── c_driver.h              # 导出声明
├── c_driver.cpp            # 导出 create_memory_reader()
├── jni/
│   ├── Android.mk          # NDK 构建配置
│   └── Application.mk      # 目标平台：arm64-v8a, API 21+
└── README.md
```

## 编译

```bash
# 需要 Android NDK
ndk-build

# 输出
libs/arm64-v8a/libsyscall.so
```

或使用一键脚本：

```bash
bash 一键构建脚本.sh
```

## 部署

编译后将 `libsyscall.so` 推送到手机：

```bash
adb push libs/arm64-v8a/libsyscall.so /data/local/tmp/libsyscall.so
```

fscan GUI 或 CLI 会通过 `--reader` 参数加载该 .so：

```bash
# CLI 方式
./scan scan -p com.game -a 0x1234 --reader /data/local/tmp/libsyscall.so

# 自测读写正确性
./scan reader-test --reader /data/local/tmp/libsyscall.so
```

## 要求

- ARM64 (aarch64) 架构
- Android 8.0+ (API 21+)
- root 权限或 ptrace 权限
