#ifndef C_DRIVER_H
#define C_DRIVER_H

#include "memory_reader_impl.h"

extern "C" {
    memtool::MemoryReader *create_memory_reader();
}

#endif // C_DRIVER_H
