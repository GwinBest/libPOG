#pragma once

#define BUFFER_MAX_SIZE 8192

typedef struct DataBuffer
{
	char data[BUFFER_MAX_SIZE];
	size_t size = 0;

}DataBuffer_t;