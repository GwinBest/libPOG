#pragma once

#define BUFFER_MAX_SIZE 4096

typedef struct DataBuffer
{
	char data[BUFFER_MAX_SIZE];
	size_t size = 0;

}DataBuffer_t;