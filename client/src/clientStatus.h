#pragma once

#include <stdint.h>

enum ClientStatus : uint8_t
{
	kClientDisconnected = 0,
	kCLientInited	    = 1,
	kClientConnected    = 2,
};