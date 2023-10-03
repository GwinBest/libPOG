#include "HTTPSClient.h"

#ifdef _WIN32 

#define WIN(exp) exp
#define NIX(exp)

#else //*nix

#define WIN(exp)
#define NIX(exp) exp

#endif


namespace Net
{
	HTTPSClient::HTTPSClient() : HTTPClient()
	{
		sslContext = nullptr;
		sslSocket = nullptr;

		clientStatus = ClientStatus::kClientDisconnected;
	}

	void HTTPSClient::Init(const uint32_t port,const std::string& hostAddress)
	{
		if ((sslContext = SSL_CTX_new(TLS_client_method())) == nullptr)
		{
			std::cout << "Failed to create SSL context" << std::endl;
			ERR_print_errors_fp(stderr);
			exit(SOCKET_ERROR);
		}

		if ((sslSocket = SSL_new(sslContext)) == nullptr)
		{
			std::cout << "Failed to create SSL socket" << std::endl;
			ERR_print_errors_fp(stderr);
			SSL_CTX_free(sslContext);
			exit(SOCKET_ERROR);
		}

		if (!Connect(port, hostAddress))				// connect to TCP socket
		{
			std::cout << "Failed to establish SSL connection" << std::endl;
			ERR_print_errors_fp(stderr);
			SSL_shutdown(sslSocket);
			SSL_free(sslSocket);
			SSL_CTX_free(sslContext);
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}

		SSL_set_fd(sslSocket, clientSocket);			//Binding an SSL socket to a TCP socket

		clientStatus = ClientStatus::kCLientInited;
	}

	bool HTTPSClient::HTTPSConnect(const uint32_t port, const std::string& hostAddress)
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			std::cout << "Connection Error: client already connected " << std::endl;
			return false;
		}

		Init(port, hostAddress);

		if (SSL_connect(sslSocket) != 1)
		{
			std::cout << "Failed to establish SSL connection" << std::endl;
			ERR_print_errors_fp(stderr);
			return false;
		}
		clientStatus = ClientStatus::kClientConnected;

		return true;
	}

	std::string HTTPSClient::SendHttpsRequest(const std::string& method, const std::string& uri, const std::string& version)
	{
		request.clear();
		response.clear();

		request = CreateRequest(const_cast<std::string&>(method), const_cast<std::string&>(uri), const_cast<std::string&>(version));

		Send();
		do
		{
			Receive();
			Proccess();
		} while (buffer.size > 0);

		return response;
	}

	void HTTPSClient::Send()
	{
		if (SSL_write(sslSocket, request.c_str(), request.size()) <= 0)
		{
			std::cout << "Failed to send HTTP request" << std::endl;
			ERR_print_errors_fp(stderr);
			SSL_shutdown(sslSocket);
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			SSL_free(sslSocket);
			SSL_CTX_free(sslContext);
			exit(SOCKET_ERROR);
		}
	}

	void HTTPSClient::Receive()
	{
		if ((buffer.size = SSL_read(sslSocket, buffer.data, BUFFER_MAX_SIZE)) < 0)
		{
			std::cout << "Receive error:" <<  std::endl;
			ERR_print_errors_fp(stderr);
			SSL_shutdown(sslSocket);
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			SSL_free(sslSocket);
			SSL_CTX_free(sslContext);
		}
	}

	void HTTPSClient::Proccess()
	{
		for (size_t i = 0; i < buffer.size; ++i)
			response += buffer.data[i];
	}

	uint8_t HTTPSClient::GetClientStatus()
	{
		return clientStatus;
	}

	char* HTTPSClient::GetIpAddress()
	{
		return ipAddress;
	}

	uint32_t HTTPSClient::GetPort()
	{
		return port;
	}

	void HTTPSClient::HTTPSDisconnect()
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			SSL_shutdown(sslSocket);
			SSL_free(sslSocket);
			SSL_CTX_free(sslContext);
			clientStatus = ClientStatus::kClientDisconnected;
		}
		HTTPClient::Disconnect();
	}

	HTTPSClient::~HTTPSClient()
	{
		HTTPSClient::Disconnect();
	}

}
