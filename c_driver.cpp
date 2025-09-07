#include "c_driver.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int c_driver::open_driver() {
    fd = open("/dev/fscan", O_RDWR);
    if (fd > 0) {
        return 1;
    }
    return 0;
}

c_driver::~c_driver() {
    if (fd > 0)
        close(fd);
}

bool c_driver::init_kernel() {
    if (fd > 0) {
        std::cout << "内核进程: " << target_pid << std::endl;
        return true;
    }
    open_driver();
    if (fd <= 0) {
        std::cout << "提示: 驱动打开失败" << std::endl;
        return false;
    }
    std::cout << "提示: 驱动打开成功 " << fd << std::endl;
    return true;
}

bool c_driver::get_init() const {
    if (fd > 0 && this->target_pid > 0)
        return true;
    return false;
}

bool c_driver::init_pid(pid_t pid) {
    this->target_pid = pid;
    return target_pid > 0;
}

bool c_driver::read(uintptr_t addr, void *buffer, size_t size) const {
    COPY_MEMORY cm{};

    cm.pid = target_pid;
    cm.addr = addr;
    cm.buffer = buffer;
    cm.size = size;

    if (ioctl(fd, OP_READ_MEM, &cm) != 0) {
        return false;
    }
    return true;
}

// 全局函数实现
c_driver driver;

extern "C" {
    bool fsinit_kernel() {
        return driver.init_kernel();
    }

    bool fsget_init() {
        return driver.get_init();
    }

    bool fsinit_pid(pid_t pid) {
        return driver.init_pid(pid);
    }

    bool fsread(uintptr_t addr, void *buffer, size_t size) {
        return driver.read(addr, buffer, size);
    }
}