#ifndef C_DRIVER_H
#define C_DRIVER_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <any>
#include <functional>
#include <unordered_map>

// 全局函数声明
extern "C" {
    bool fsinit_kernel();
    bool fsget_init();
    bool fsinit_pid(pid_t pid);
    bool fsread(uintptr_t addr, void *buffer, size_t size);
}

#endif // C_DRIVER_H