#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>

#include <openssl/ssl.h>

#include "HTTPClient.h"

namespace Net
{
	class HTTPSClient final : HTTPClient
	{
	private:
		SSL_CTX* sslContext;
		SSL* sslSocket;
		uint8_t clientStatus;

		std::string request;
		std::string response;
		DataBuffer_t buffer;

	public:
		HTTPSClient();
		~HTTPSClient();

		bool HTTPSConnect(const uint32_t port, const std::string& hostAddress);
		std::string SendHttpsRequest(const std::string& method, const std::string& uri, const std::string& version);
		void HTTPSDisconnect();

		uint8_t GetClientStatus();
		char* GetIpAddress();
		uint32_t GetPort();

	private:
		void Init(const uint32_t port, const std::string& hostAddress);
		using HTTPClient::CreateRequest;
		void Receive();
		void Proccess();
		void Send();
	};
}

#endif // !HTTPSCLIENT_H

