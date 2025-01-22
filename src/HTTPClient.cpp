#include "HTTPClient.h"

#include <iostream>

#include "stringUtils.h"

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

using namespace Net;

HTTPClient::HTTPClient() {
    infoLength = sizeof(*socketInfo.IPv4);
    hints.ai_family = AF_UNSPEC;
    memset(&hints, 0, sizeof(hints));
}

void HTTPClient::Init(const uint32_t port, const std::string_view hostAddress) {
    this->port = port;
    this->hostAddress = hostAddress;

    WIN(
        if (WSAStartup(DLLVersion, &wsaData)) {
            std::cout << "WSA startup failed" << std::endl;
            exit(SOCKET_ERROR);
        }
    );

    if (getaddrinfo(this->hostAddress.c_str(), NULL, &hints, &result)) {
        std::cout << "getaddrinfo() failed" << std::endl;
        WIN(WSACleanup());
        freeaddrinfo(result);
        exit(SOCKET_ERROR);
    }

    if (result->ai_family == AF_INET) {
        socketInfo.IPv4 = reinterpret_cast<SOCKADDR_IN *>(result->ai_addr);
        inet_ntop(socketInfo.IPv4->sin_family, &socketInfo.IPv4->sin_addr, this->ipAddress, IPV4_LENGTH);
    }
    else if (result->ai_family == AF_INET6) {
        socketInfo.IPv6 = reinterpret_cast<SOCKADDR_IN6 *>(result->ai_addr);
        inet_ntop(socketInfo.IPv6->sin6_family, &socketInfo.IPv6->sin6_addr, this->ipAddress, IPV6_LENGTH);
    }
    socketInfo.IPv4->sin_port = htons(this->port);

    if (((clientSocket = socket(socketInfo.IPv4->sin_family, SOCK_STREAM, NULL)) == SOCKET_ERROR)) {
        std::cout << "Client socket init failed" << std::endl;
        WIN(WSACleanup());
        freeaddrinfo(result);
        exit(SOCKET_ERROR);
    }

    clientStatus = ClientStatus::kCLientInited;
}

/// <summary>
/// HTTP port - 80
/// HTTPS port - 443
/// </summary>
bool HTTPClient::Connect(const uint32_t port, const std::string_view hostAddress) {
    if (clientStatus != ClientStatus::kClientDisconnected) {
        std::cout << "Connection Error: client already connected " << std::endl;
        return false;
    }

    Init(port, hostAddress);

    if (connect(clientSocket, reinterpret_cast<SOCKADDR *>(socketInfo.IPv4), sizeof(*socketInfo.IPv4))) {
        std::cout << "Connection Error " << ConvertError() << std::endl;
        return false;
    }
    clientStatus = ClientStatus::kClientConnected;

    return true;
}

std::string HTTPClient::SendHttpRequest(const std::string_view method, const std::string_view uri, const std::string_view version) {
    response.clear();
    request.clear();
    request = CreateRequest(method.data(), uri.data(), version.data());

    Send();
    do {
        Receive();
        Proccess();
    } while (buffer.size > 0);

    return response;
}

inline std::string HTTPClient::CreateRequest(std::string method, const std::string_view uri, const std::string_view version) {
    return request = ((method = StringUtils::ToUpper(StringUtils::Trim(method))) == "GET") ?
        method + " " + StringUtils::Trim(uri) + "/ HTTP/" + StringUtils::Trim(version) + "\r\n" +
            "Host:" + StringUtils::ToLower(StringUtils::Trim(hostAddress)) + "\r\n" +
            "Connection: close\r\n\r\n"
        :
        method + " " + StringUtils::Trim(uri) + "/ HTTP/" + StringUtils::Trim(version) + "\r\n" +
            "Host:" + StringUtils::ToLower(StringUtils::Trim(hostAddress)) + "\r\nConnection: close\r\n" +
            "Content-Type: text/html\r\n\r\n";
}

void HTTPClient::Send() {
    if ((send(clientSocket, request.c_str(), request.size(), NULL) != SOCKET_ERROR)) return;

    std::cout << "Send error:" << ConvertError() << std::endl;
    WIN(WSACleanup());
    WIN(closesocket)
    NIX(close)(clientSocket);
    freeaddrinfo(result);
    exit(SOCKET_ERROR);
}

void HTTPClient::Receive() {
    if ((buffer.size = recv(clientSocket, buffer.data, BUFFER_MAX_SIZE - 1, NULL)) != SOCKET_ERROR) return;

    std::cout << "Receive error:" << ConvertError() << std::endl;
    WIN(WSACleanup());
    WIN(closesocket)
    NIX(close)(clientSocket);
    freeaddrinfo(result);
    exit(SOCKET_ERROR);
}

void HTTPClient::Proccess() {
    response.assign(buffer.data);
}

void HTTPClient::Disconnect() {
    if (clientStatus == ClientStatus::kClientDisconnected) return;

    WIN(WSACleanup());
    WIN(closesocket)
    NIX(close)(clientSocket);
    freeaddrinfo(result);
    clientStatus = ClientStatus::kClientDisconnected;
}

HTTPClient::~HTTPClient() {
    HTTPClient::Disconnect();
}

uint8_t HTTPClient::GetClientStatus() {
    return clientStatus;
}

char *HTTPClient::GetIpAddress() {
    return ipAddress;
}

uint32_t HTTPClient::GetPort() {
    return port;
}
