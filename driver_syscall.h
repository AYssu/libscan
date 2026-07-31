#ifndef DRIVER_SYSCALL_H
#define DRIVER_SYSCALL_H

#include "driver_base.h"
#include <sys/syscall.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>

/**
 * Syscall 驱动
 * 使用 process_vm_readv / process_vm_writev 直接读写目标进程内存
 * 无需内核模块，需要 root 或 ptrace 权限
 */
class syscall_driver : public driver_base {
public:
    syscall_driver() = default;

    ~syscall_driver() override = default;

    bool set_pid(pid_t pid) override {
        target_pid = pid;
        return target_pid > 0;
    }

    bool read(uintptr_t address, void *buffer, size_t size) override {
        if (target_pid <= 0 || !buffer || size == 0) return false;

        struct iovec local;
        local.iov_base = buffer;
        local.iov_len = size;

        struct iovec remote;
        remote.iov_base = reinterpret_cast<void *>(address);
        remote.iov_len = size;

        ssize_t n = syscall(SYS_process_vm_readv, target_pid, &local, 1, &remote, 1, 0);
        return n == static_cast<ssize_t>(size);
    }

    bool write(uintptr_t address, void *buffer, size_t size) override {
        if (target_pid <= 0 || !buffer || size == 0) return false;

        struct iovec local;
        local.iov_base = buffer;
        local.iov_len = size;

        struct iovec remote;
        remote.iov_base = reinterpret_cast<void *>(address);
        remote.iov_len = size;

        ssize_t n = syscall(SYS_process_vm_writev, target_pid, &local, 1, &remote, 1, 0);
        return n == static_cast<ssize_t>(size);
    }

    uintptr_t get_module_base(const char *name) override {
        if (target_pid <= 0 || !name) return 0;

        char path[64];
        snprintf(path, sizeof(path), "/proc/%d/maps", target_pid);

        FILE *f = fopen(path, "r");
        if (!f) return 0;

        char line[512];
        uintptr_t base = 0;

        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, name)) {
                // 解析第一段地址范围的起始地址
                // 格式: "7f12340000-7f12350000 r-xp 00000000 ..."
                char *dash = strchr(line, '-');
                if (dash) {
                    *dash = '\0';
                    base = strtoull(line, nullptr, 16);
                    break;
                }
            }
        }

        fclose(f);
        return base;
    }
};

#endif // DRIVER_SYSCALL_H
