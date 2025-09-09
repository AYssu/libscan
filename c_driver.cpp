#include "c_driver.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "driver.h"

// 全局函数实现
Driver *driver = new Driver();
extern "C" {
    bool fsinit_kernel() {
        return true;
    }

    bool fsget_init() {
        return true;
    }

    bool fsinit_pid(pid_t pid) {
        return driver->initpid(pid);
    }

    bool fsread(uintptr_t addr, void *buffer, size_t size) {
        return driver->read(addr, buffer, size);
    }
}