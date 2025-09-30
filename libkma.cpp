#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "driver.h"
#include <cstdint>
#define  PAGE_SIZE 4096

/**
 * 来自开源https://github.com/AYssu/Gg_Docking_Kernel.git
 */
static uintptr_t get_module_base(const char* module_name) {
    FILE* file = fopen("/proc/self/maps", "r");
    if (!file) {
        return 0;
    }
    uintptr_t base = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, module_name) && !strstr(line, "r-xp")) {
            sscanf(line, "%lx", &base);
            break;
        }
    }
    fclose(file);
    return base;
}
// 全局函数实现
Driver *driver = new Driver();
long my_syscall(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {

    // 判断syscall读取方式拦截替换
    if (number == SYS_process_vm_readv) { // SYS_process_vm_readv
        int target_pid = (int)arg1;
        struct iovec* local_iov = (struct iovec*)arg2;
        long local_count = arg3;
        struct iovec* remote_iov = (struct iovec*)arg4;
        long remote_count = arg5;
        
        // 内核驱动初始化目标包名进程pid
        driver->initpid(target_pid);
        ssize_t total_read = 0;
        for (long i = 0; i < local_count && i < remote_count; ++i) {
            uintptr_t remote_addr = (uintptr_t)remote_iov[i].iov_base;
            void* local_buffer = local_iov[i].iov_base;
            size_t size = local_iov[i].iov_len;
			         driver->read(remote_addr, local_buffer, size);
			         total_read += size;
		      }
        return total_read;
    }
    if (number == SYS_process_vm_writev) { // write
        int target_pid = (int)arg1;
        struct iovec* local_iov = (struct iovec*)arg2;
        long local_count = arg3;
        struct iovec* remote_iov = (struct iovec*)arg4;
        long remote_count = arg5;
        
        // 内核驱动初始化目标包名进程pid
        driver->initpid(target_pid);
        ssize_t total_read = 0;
        for (long i = 0; i < local_count && i < remote_count; ++i) {
            uintptr_t remote_addr = (uintptr_t)remote_iov[i].iov_base;
            void* local_buffer = local_iov[i].iov_base;
            size_t size = local_iov[i].iov_len;
			         driver->write(remote_addr, local_buffer, size);
			         total_read += size;
		      }
        return total_read;
    }

    // 其他系统调用保持原样
    return syscall(number, arg1, arg2, arg3, arg4, arg5, arg6);
}



__attribute__((constructor)) static void init() {
    auto base = get_module_base("lib5.so");
    for (long i = 0; ; ++i) {
        auto addr = base + i * sizeof(uintptr_t);
        if (*(uintptr_t*)addr == (uintptr_t)syscall) {
            //对齐
            mprotect((void*)(addr & ~(PAGE_SIZE - 1)), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
            *(uintptr_t*)(addr) = (uintptr_t)my_syscall;
            break;
        }
    }
}
