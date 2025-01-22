#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>
#include <cstdint>

static constexpr uint16_t BUFFER_MAX_SIZE = 8192;

struct DataBuffer {
    char data[BUFFER_MAX_SIZE];
    size_t size = 0;
};

#endif // !DATA_BUFFER_H