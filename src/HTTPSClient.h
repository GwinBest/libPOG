#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>

#include <openssl/ssl.h>

#include "HTTPClient.h"

namespace Net {
    class HTTPSClient final : HTTPClient {
    public:
        HTTPSClient();
        ~HTTPSClient() override { HTTPSClient::Disconnect(); }

        bool HTTPSConnect(const uint32_t port, const std::string& hostAddress);
        std::string SendHttpsRequest(const std::string& method, const std::string& uri, const std::string& version);
        void HTTPSDisconnect();

        inline uint8_t GetClientStatus() { return clientStatus; };
        inline char* GetIpAddress()      { return ipAddress; };
        inline uint32_t GetPort()        { return port; };

    private:
        using HTTPClient::CreateRequest;

        void Init(const uint32_t port, const std::string& hostAddress);
        void Receive();
        void Proccess();
        void Send();

    private:
        SSL_CTX* sslContext;
        SSL* sslSocket;
        uint8_t clientStatus;

        std::string request;
        std::string response;
        DataBuffer buffer;
    };
}

#endif // !HTTPSCLIENT_H

