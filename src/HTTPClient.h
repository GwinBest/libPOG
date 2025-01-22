#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#ifdef _WIN32 // Windows NT
#include <WinSock2.h>
#include <WS2tcpip.h>
#else  //*nix
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <string>

#include "dataBuffer.h"

#ifdef _WIN32 // Windows NT
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in6 SOCKADDR_IN6;
#else // POSIX
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in6 SOCKADDR_IN6;
typedef struct addrinfo ADDRINFO;
typedef int SOCKET;
#endif

#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
#endif

#ifndef INVALID_SOCKET
    #define INVALID_SOCKET 0
#endif

#define INVALID_PORT  0
#ifndef SOCKET_ERROR
    #define SOCKET_ERROR -1
#endif

static constexpr uint8_t IPV4_LENGTH = 15;
static constexpr uint8_t IPV6_LENGTH = 65;

static constexpr uint8_t  HTTP_PORT = 80;
static constexpr uint16_t HTTPS_PORT = 443;

namespace Net {
    union SocketInfo {
        SOCKADDR_IN* IPv4 = nullptr;
        SOCKADDR_IN6* IPv6;
    };

    class HTTPClient {
    public:
        HTTPClient();
        virtual ~HTTPClient() { HTTPClient::Disconnect(); }

        bool Connect(const uint32_t port, const std::string_view hostAddress);
        std::string SendHttpRequest(const std::string_view method, const std::string_view uri, const std::string_view version);
        void Disconnect();

        inline uint8_t GetClientStatus() { return clientStatus; }
        inline char* GetIpAddress()      { return ipAddress; }
        inline uint32_t GetPort()        { return port; }

    protected:
        std::string CreateRequest(std::string method, const std::string_view uri, const std::string_view version);

    protected:
        enum ClientStatus : uint8_t {
            kClientDisconnected = 0,
            kCLientInited = 1,
            kClientConnected = 2,
        };

#ifdef _WIN32
        WSADATA wsaData = {};
        static const WORD DLLVersion = MAKEWORD(2, 2);
#endif

        SOCKET clientSocket = INVALID_SOCKET;
        ADDRINFO hints = {};
        ADDRINFO* result = nullptr;
        SocketInfo socketInfo = {};
        uint32_t infoLength = 0;

        char ipAddress[IPV6_LENGTH] = "";
        std::string hostAddress;
        uint32_t port = ntohs(INVALID_PORT);

    private:
        void Init(const uint32_t port, const std::string_view hostAddress);
        void Receive();
        void Proccess();
        void Send();

    private:
        std::string request;
        std::string response;
        DataBuffer buffer = {};

        ClientStatus clientStatus = ClientStatus::kClientDisconnected;
    };
}

#endif // !HTTPClient_H
