#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "driver.h"
#include "Log.h"
#define  PAGE_SIZE 4096
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
    if (number == 270) { // SYS_process_vm_readv
        int target_pid = (int)arg1;
        struct iovec* local_iov = (struct iovec*)arg2;
        long local_count = arg3;
        struct iovec* remote_iov = (struct iovec*)arg4;
        long remote_count = arg5;

        LOGD("驱动读取: pid=%d, local_count=%ld, remote_count=%ld", target_pid, local_count, remote_count);
        
        // 内核驱动初始化目标包名进程pid
        driver->initpid(target_pid);
        ssize_t total_read = 0;
        for (long i = 0; i < local_count && i < remote_count; ++i) {
            uintptr_t remote_addr = (uintptr_t)remote_iov[i].iov_base;
            void* local_buffer = local_iov[i].iov_base;
            size_t size = local_iov[i].iov_len;

            // 使用驱动读取内存
			         driver->read(remote_addr, local_buffer, size);
			         //LOGE("读取成功: addr=0x%lx, size=%zu", remote_addr, size);
			         total_read += size;
		      }
        return total_read;
    }
    // 其他系统调用保持原样
    return syscall(number, arg1, arg2, arg3, arg4, arg5, arg6);
}



__attribute__((constructor)) static void init() {
    auto base = get_module_base("lib5.so");
    LOGD("Module base address: %lx", base);
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
