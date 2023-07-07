#pragma once

#include <stdint.h>
#include <WS2tcpip.h>
#include <WinSock2.h>

#include <cassert>
#include <iostream>

#include "bufer.h"
#include "clientStatus.h"
#include "string.h"


#pragma comment(lib, "ws2_32.lib")


#define INVALID_PORT -1

#define IPV4_LENGTH 15
#define IPV6_LENGTH 65

#define HTTP_PORT  80
#define HTTPS_PORT 443


namespace Net
{
	typedef union SocketInfo
	{
		SOCKADDR_IN* IPv4;
		SOCKADDR_IN6* IPv6;

	}SocketInfo_t;


	class Client
	{
	private:
		WSADATA wsaData;
		const WORD DLLVersion = MAKEWORD(2, 2);
		SOCKET clientSocket;
		ADDRINFO hints;
		ADDRINFO* result;
		SocketInfo_t socketInfo;
		uint32_t infoLength;

		std::string ipAddress;
		std::string hostAddress;
		uint32_t port;

		std::string request;
		std::string response;
		DataBuffer_t buffer;
	
		uint8_t clientStatus;
		
	public:
		Client();
		~Client();

		void Connect(const uint32_t port, const std::string& hostAddress);
		std::string SendHttpRequest(const std::string& method, const std::string& uri, const std::string& version);
		void Disconnect();

		inline uint8_t GetClientStatus();
		inline std::string GetIpAddress();
		inline uint32_t GetPort();

	private:
		void Init(const uint32_t port, const std::string& hostAddress);
		inline std::string CreateRequest(std::string& method, std::string& uri, std::string& version);
		void Receive();
		void Proccess();
		void Send();
	};



}
