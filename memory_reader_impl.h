#ifndef MEMORY_READER_IMPL_H
#define MEMORY_READER_IMPL_H

// MemoryReader interface — 匹配 memtool::MemoryReader 布局
// 避免 include memtool 头文件，直接定义兼容接口

#include <cstdint>
#include <cstddef>
#include <string>

namespace memtool {

class MemoryReader {
public:
    virtual ~MemoryReader() = default;
    virtual bool init(pid_t pid) = 0;
    virtual bool read(uintptr_t addr, void *buffer, size_t size) = 0;
    virtual bool write(uintptr_t addr, const void *buffer, size_t size) = 0;
    virtual bool is_page_resident(uintptr_t addr) = 0;
    virtual size_t *check_pages(uintptr_t addr, size_t size) = 0;
    virtual void close() = 0;
    virtual const char *name() const = 0;
};

}  // namespace memtool

#endif // MEMORY_READER_IMPL_H
