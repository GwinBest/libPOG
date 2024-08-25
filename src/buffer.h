#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_MAX_SIZE 8192

struct DataBuffer
{
    char data[BUFFER_MAX_SIZE];
    size_t size = 0;
};
#endif // !BUFFER_H