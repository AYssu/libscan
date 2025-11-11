#ifndef DRIVER_RT_HOOKPRO_H
#define DRIVER_RT_HOOKPRO_H

#include "driver_base.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <iostream>

/**
 * RT HookPro 驱动或 GT2.2 驱动
 * 使用 socket 方式通信
 */
class rt_hookpro_driver : public driver_base {
private:
    int fd;
    
    typedef struct _COPY_MEMORY {
        pid_t pid;
        uintptr_t addr;
        void* buffer;
        size_t size;
    } COPY_MEMORY;
    
    typedef struct _MODULE_BASE {
        pid_t pid;
        char* name;
        uintptr_t base;
    } MODULE_BASE;
    
    enum OPERATIONS {
        OP_READ_MEM = 601,
        OP_WRITE_MEM = 602,
        OP_MODULE_BASE = 603
    };

public:
    rt_hookpro_driver() {
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd == -1) {
            perror("[-] RT HookPro驱动打开失败");
        } else {
            std::cout << "[+] RT HookPro驱动加载成功" << std::endl;
        }
    }
    
    ~rt_hookpro_driver() override {
        if (fd > 0) {
            close(fd);
            std::cout << "[-] RT HookPro驱动关闭成功" << std::endl;
        }
    }
    
    bool set_pid(pid_t pid) override {
        target_pid = pid;
        return target_pid > 0;
    }
    
    bool read(uintptr_t address, void* buffer, size_t size) override {
        if (fd <= 0) return false;
        
        COPY_MEMORY cm;
        cm.pid = target_pid;
        cm.addr = address;
        cm.buffer = buffer;
        cm.size = size;
        
        return ioctl(fd, OP_READ_MEM, &cm) == 0;
    }
    
    bool write(uintptr_t address, void* buffer, size_t size) override {
        if (fd <= 0) return false;
        
        COPY_MEMORY cm;
        cm.pid = target_pid;
        cm.addr = address;
        cm.buffer = buffer;
        cm.size = size;
        
        return ioctl(fd, OP_WRITE_MEM, &cm) == 0;
    }
    
    uintptr_t get_module_base(const char* name) override {
        if (fd <= 0) return 0;
        
        MODULE_BASE mb;
        char buf[0x100];
        strncpy(buf, name, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        
        mb.pid = target_pid;
        mb.name = buf;
        
        if (ioctl(fd, OP_MODULE_BASE, &mb) != 0) {
            return 0;
        }
        
        return mb.base;
    }
};

#endif // DRIVER_RT_HOOKPRO_H

