#include "c_driver.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "driver_rt_hookpro.h"

// 全局函数实现
auto *driver = new rt_hookpro_driver();
extern "C" {
    bool fsinit_kernel() {
        return true;
    }

    bool fsget_init() {
        return true;
    }

    bool fsinit_pid(pid_t pid) {
        driver->set_pid(pid);
        return true;
    }

    bool fsread(uintptr_t addr, void *buffer, size_t size) {
        return driver->read(addr, buffer, size);
    }
}