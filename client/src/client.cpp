#include "client.h"

#pragma warning(disable:4996)


namespace Net
{
	Client::Client()
	{
		result = nullptr;
		socketInfo.IPv4 = nullptr;
		port = INVALID_PORT;
		clientSocket = INVALID_SOCKET;
		infoLength = sizeof(*socketInfo.IPv4);
		clientStatus = ClientStatus::kClientDisconnected;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
	}

	void Client::Init(const uint32_t port, const std::string& hostAddress)
	{
		this->port = port;
		this->hostAddress = hostAddress;
		
		std::cout << "WSA init" << std::endl;
		if (WSAStartup(DLLVersion, &wsaData))
		{
			std::cout << "WSA startup failed" << std::endl;
			exit(SOCKET_ERROR);
		}
		std::cout << "WSA create" << std::endl;

		if (getaddrinfo(hostAddress.c_str(), NULL, &hints, &result))
		{
			std::cout << "getaddrinfo() failed" << std::endl;
			WSACleanup();
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}

		if (result->ai_family == AF_INET)
		{
			socketInfo.IPv4 = (SOCKADDR_IN*)result->ai_addr;
			inet_ntop(socketInfo.IPv4->sin_family, &socketInfo.IPv4->sin_addr, const_cast<char*>(ipAddress.c_str()), IPV4_LENGTH);
		}
		else if (result->ai_family == AF_INET6)
		{
			socketInfo.IPv6 = (SOCKADDR_IN6*)result->ai_addr;
			inet_ntop(socketInfo.IPv6->sin6_family, &socketInfo.IPv6->sin6_addr, const_cast<char*>(ipAddress.c_str()), IPV6_LENGTH);
		}
		socketInfo.IPv4->sin_port = htons(this->port);
		
		std::cout << "Socket init" << std::endl;
		if (((clientSocket = socket(socketInfo.IPv4->sin_family, SOCK_STREAM, NULL)) == SOCKET_ERROR))
		{
			std::cout << "Client socket init failed" << std::endl;
			WSACleanup();
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}
		std::cout << "Socket inited" << std::endl;

		std::cout << "Client connected to " << ipAddress << ":" << this->port << std::endl;
	}
	
	/// <summary>
	/// HTTP port - 80
	/// HTTPS port - 443
	/// </summary>
	void Client::Connect(const uint32_t port, const std::string& hostAddress)
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			std::cout << "Connection Error: client already connected " << std::endl;
			exit(clientStatus);
		}

		Init(port, hostAddress);
		clientStatus = ClientStatus::kCLientInited;

		connect(clientSocket, (SOCKADDR*)socketInfo.IPv4, sizeof(*socketInfo.IPv4));
		clientStatus = ClientStatus::kClientConnected;
	}

	std::string Client::SendHttpRequest(const std::string& method, const std::string& uri, const std::string& version)
	{
		request = CreateRequest(const_cast<std::string&>(method), const_cast<std::string&>(uri), const_cast<std::string&>(version));

		Send();
		do
		{
			Receive();
			Proccess();
		} while (buffer.size > 0);

		return response;
	}

	inline std::string Client::CreateRequest(std::string& method, std::string& uri, std::string& version)
	{
		return request = ((method = ToUpper(Trim(method))) == "GET") ?

			method + " " + Trim(uri) + "/ HTTP/" + Trim(version) + "\r\n" +
			"Host:" + ToLower(Trim(hostAddress)) + "\r\nConnection: close\r\n\r\n" :

			method + " " + Trim(uri) + "/ HTTP/" + Trim(version) + "\r\n" +
			"Host:" + ToLower(Trim(hostAddress)) + "\r\nConnection: close\r\n" +
			"Content-Type: application/json\r\n\r\n";
	}
	
	void Client::Send()
	{
		if ((send(clientSocket, request.c_str(), request.size(), NULL) == SOCKET_ERROR))
		{
			std::cout << "Send error:" << WSAGetLastError() << std::endl;
			WSACleanup();
			closesocket(clientSocket);
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}
	}

	void Client::Receive()
	{
		if ((buffer.size = recv(clientSocket, buffer.data, BUFFER_MAX_SIZE - 1, NULL)) == SOCKET_ERROR)
		{
			std::cout << "Receive error:" << WSAGetLastError() << std::endl;
			WSACleanup();
			closesocket(clientSocket);
			freeaddrinfo(result);
			exit(SOCKET_ERROR);
		}
	}

	void Client::Proccess()
	{
		for (size_t i = 0; i < buffer.size; i++)
		{
			response += buffer.data[i];
			std::cout << buffer.data[i];
		}
	}

	void Client::Disconnect()
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			WSACleanup();
			freeaddrinfo(result);
			closesocket(clientSocket);
			clientStatus = ClientStatus::kClientDisconnected;
		}
	}

	Client::~Client()
	{
		if (clientStatus != ClientStatus::kClientDisconnected)
		{
			WSACleanup();
			freeaddrinfo(result);
			closesocket(clientSocket);
			clientStatus = ClientStatus::kClientDisconnected;
		}
	}

	inline uint8_t Client::GetClientStatus()
	{
		return clientStatus;
	}

	inline std::string Client::GetIpAddress()
	{
		return ipAddress;
	}

	inline uint32_t Client::GetPort()
	{
		return port;
	}

}