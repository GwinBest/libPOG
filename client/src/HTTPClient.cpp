#include "HTTPClient.h"

#if _MSC_VER && !__INTEL_COMPILER

#pragma warning(disable:4996)

#endif

#ifdef _WIN32 

#define WIN(exp) exp
#define NIX(exp)

// convert WinSock code error to Posix error
inline int ConvertError()
{
	switch (WSAGetLastError())
	{
	case 0:
		return 0;
	case WSAEINTR:
		return EINTR;
	case WSAEINVAL:
		return EINVAL;
	case WSA_INVALID_HANDLE:
		return EBADF;
	case WSA_NOT_ENOUGH_MEMORY:
		return ENOMEM;
	case WSA_INVALID_PARAMETER:
		return EINVAL;
	case WSAENAMETOOLONG:
		return ENAMETOOLONG;
	case WSAENOTEMPTY:
		return ENOTEMPTY;
	case WSAEWOULDBLOCK:
		return EAGAIN;
	case WSAEINPROGRESS:
		return EINPROGRESS;
	case WSAEALREADY:
		return EALREADY;
	case WSAENOTSOCK:
		return ENOTSOCK;
	case WSAEDESTADDRREQ:
		return EDESTADDRREQ;
	case WSAEMSGSIZE:
		return EMSGSIZE;
	case WSAEPROTOTYPE:
		return EPROTOTYPE;
	case WSAENOPROTOOPT:
		return ENOPROTOOPT;
	case WSAEPROTONOSUPPORT:
		return EPROTONOSUPPORT;
	case WSAEOPNOTSUPP:
		return EOPNOTSUPP;
	case WSAEAFNOSUPPORT:
		return EAFNOSUPPORT;
	case WSAEADDRINUSE:
		return EADDRINUSE;
	case WSAEADDRNOTAVAIL:
		return EADDRNOTAVAIL;
	case WSAENETDOWN:
		return ENETDOWN;
	case WSAENETUNREACH:
		return ENETUNREACH;
	case WSAENETRESET:
		return ENETRESET;
	case WSAECONNABORTED:
		return ECONNABORTED;
	case WSAECONNRESET:
		return ECONNRESET;
	case WSAENOBUFS:
		return ENOBUFS;
	case WSAEISCONN:
		return EISCONN;
	case WSAENOTCONN:
		return ENOTCONN;
	case WSAETIMEDOUT:
		return ETIMEDOUT;
	case WSAECONNREFUSED:
		return ECONNREFUSED;
	case WSAELOOP:
		return ELOOP;
	case WSAEHOSTUNREACH:
		return EHOSTUNREACH;
	default:
		return EIO;
	}
}

#else //*nix

#define WIN(exp)
#define NIX(exp) exp

#endif

namespace Net
{
	HTTPClient::HTTPClient()
	{
		result = nullptr;
		socketInfo.IPv4 = nullptr; 
		port = ntohs(INVALID_PORT);
		clientSocket = INVALID_SOCKET;
		infoLength = sizeof(*socketInfo.IPv4);
		clientStatus = ClientStatus::kClientDisconnected;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
	}

	void HTTPClient::Init(const uint32_t port, const std::string& hostAddress)
	{
		this->port = port;
		this->hostAddress = hostAddress;
		
		WIN(
		std::cout << "WSA init" << std::endl;
		if (WSAStartup(DLLVersion, &wsaData))
		{
			std::cout << "WSA startup failed" << std::endl;
			exit(SOCKET_ERROR);
		}
		std::cout << "WSA create" << std::endl;
		);

		if (getaddrinfo(hostAddress.c_str(), NULL, &hints, &result))
		{
			std::cout << "getaddrinfo() failed" << std::endl;
			WIN(WSACleanup());
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}

		if (result->ai_family == AF_INET)
		{
			socketInfo.IPv4 = (SOCKADDR_IN*)result->ai_addr;
			inet_ntop(socketInfo.IPv4->sin_family, &socketInfo.IPv4->sin_addr, this->ipAddress, IPV4_LENGTH);
		}
		else if (result->ai_family == AF_INET6)
		{
			socketInfo.IPv6 = (SOCKADDR_IN6*)result->ai_addr;
			inet_ntop(socketInfo.IPv6->sin6_family, &socketInfo.IPv6->sin6_addr, this->ipAddress, IPV6_LENGTH);
		}
		socketInfo.IPv4->sin_port = htons(this->port);
		
		std::cout << "Socket init" << std::endl;
		if (((clientSocket = socket(socketInfo.IPv4->sin_family, SOCK_STREAM, NULL)) == SOCKET_ERROR))
		{
			std::cout << "Client socket init failed" << std::endl;
			WIN(WSACleanup());
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}
		std::cout << "Socket inited" << std::endl;

		std::cout << "Client connected to " << this->ipAddress << ":" << this->port << std::endl;
		clientStatus = ClientStatus::kCLientInited;
	}
	
	/// <summary>
	/// HTTP port - 80
	/// HTTPS port - 443
	/// </summary>
	bool HTTPClient::Connect(const uint32_t port, const std::string& hostAddress)
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			std::cout << "Connection Error: client already connected " << std::endl;
			return false;
		}

		Init(port, hostAddress);
		
		if (connect(clientSocket, (SOCKADDR*)socketInfo.IPv4, sizeof(*socketInfo.IPv4)))
		{
			std::cout << "Connection Error " << ConvertError() << std::endl;
			return false;
		}
		clientStatus = ClientStatus::kClientConnected;

		return true;
	}

	std::string HTTPClient::SendHttpRequest(const std::string& method, const std::string& uri, const std::string& version)
	{
		response.clear();
		request.clear();
		request = CreateRequest(const_cast<std::string&>(method), const_cast<std::string&>(uri), const_cast<std::string&>(version));

		Send();
		do
		{
			Receive();
			Proccess();
		} while (buffer.size > 0);

		return response;
	}

	inline std::string HTTPClient::CreateRequest(std::string& method, std::string& uri, std::string& version)
	{
		return request = ((method = ToUpper(Trim(method))) == "GET") ?

			method + " " + Trim(uri) + "/ HTTP/" + Trim(version) + "\r\n" +
			"Host:" + ToLower(Trim(hostAddress)) + "\r\n" +
			"Connection: close\r\n\r\n" :

			method + " " + Trim(uri) + "/ HTTP/" + Trim(version) + "\r\n" +
			"Host:" + ToLower(Trim(hostAddress)) + "\r\nConnection: close\r\n" +
			"Content-Type: text/html\r\n\r\n";
	}
	
	void HTTPClient::Send()
	{
		if ((send(clientSocket, request.c_str(), request.size(), NULL) == SOCKET_ERROR))
		{
			std::cout << "Send error:" << ConvertError() << std::endl;
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}
	}

	void HTTPClient::Receive()
	{
		if ((buffer.size = recv(clientSocket, buffer.data, BUFFER_MAX_SIZE - 1, NULL)) == SOCKET_ERROR)
		{
			std::cout << "Receive error:" << ConvertError() << std::endl;
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}
	}

	void HTTPClient::Proccess()
	{
		for (size_t i = 0; i < buffer.size; ++i)
		{
			response += buffer.data[i];
		}
	}

	void HTTPClient::Disconnect()
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			clientStatus = ClientStatus::kClientDisconnected;
		}
	}

	HTTPClient::~HTTPClient()
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			WIN(WSACleanup());
			WIN(closesocket)NIX(close)(clientSocket);
			freeaddrinfo(result);
			clientStatus = ClientStatus::kClientDisconnected;
		}
	}

	uint8_t HTTPClient::GetClientStatus()
	{
		return clientStatus;
	}

	char* HTTPClient::GetIpAddress()
	{
		return ipAddress;
	}

	uint32_t HTTPClient::GetPort()
	{
		return port;
	}

}


