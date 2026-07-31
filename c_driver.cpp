#include "c_driver.h"
#include "memory_reader_impl.h"
#include "driver_syscall.h"

// ---- MemoryReader 包装：将 syscall_driver 适配为 MemoryReader 接口 ----

class DriverMemoryReader : public memtool::MemoryReader {
public:
    bool init(pid_t pid) override { return driver_.set_pid(pid); }
    bool read(uintptr_t addr, void *buffer, size_t size) override { return driver_.read(addr, buffer, size); }
    bool write(uintptr_t addr, const void *buffer, size_t size) override { return driver_.write(addr, const_cast<void *>(buffer), size); }
    bool is_page_resident(uintptr_t) override { return false; }
    size_t *check_pages(uintptr_t, size_t) override { return nullptr; }
    void close() override {}
    const char *name() const override { return "syscall-plugin"; }

private:
    syscall_driver driver_;
};

// ---- 导出接口 ----

static DriverMemoryReader g_reader;

extern "C" {
    memtool::MemoryReader *create_memory_reader() { return &g_reader; }
}
