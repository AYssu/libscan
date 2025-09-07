#ifndef C_DRIVER_H
#define C_DRIVER_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <any>
#include <functional>
#include <unordered_map>

class c_driver {
private:
    int fd = -1;
    pid_t target_pid = -1;

    struct COPY_MEMORY {
        pid_t pid;
        uintptr_t addr;
        void *buffer;
        size_t size;
    };

    enum OPERATIONS {
        OP_READ_MEM = 777,
        OP_WRITE_MEM = 778,
    };

    int open_driver();

public:
    ~c_driver();
    bool init_kernel();
    bool get_init() const;
    bool init_pid(pid_t pid);
    bool read(uintptr_t addr, void *buffer, size_t size) const;
};

// 全局函数声明
extern "C" {
    bool fsinit_kernel();
    bool fsget_init();
    bool fsinit_pid(pid_t pid);
    bool fsread(uintptr_t addr, void *buffer, size_t size);
}

#endif // C_DRIVER_H