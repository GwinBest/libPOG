#include "../src/HTTP/HTTPClient.h"
#include "../src/HTTPS/HTTPSClient.h"

#include <locale>


int main()
{
    //---https client---//

    Net::HTTPSClient httpClient;
    httpClient.HTTPSConnect(HTTPS_PORT, "xserver.ru");
    std::string response = httpClient.SendHttpsRequest("get", "/computer/servers/razn/29/", "1.1");


    std::cout << "Response 1: " << response << std::endl;
    response.clear();
    httpClient.HTTPSDisconnect();

    httpClient.HTTPSConnect(HTTPS_PORT, "xserver.ru");
    response = httpClient.SendHttpsRequest("get", "/computer/servers/razn/28/", "1.1");

    std::cout << "Response 2: " << response << std::endl;
    response.clear();
    httpClient.HTTPSDisconnect();

    //---http client---//

    Net::HTTPClient client;
    client.Connect(HTTP_PORT, "www.xserver.ru");
    response = client.SendHttpRequest("GET", "/computer/servers/razn/29/", "1.1");

    std::cout << "Response 1: " << response << std::endl;
    client.Disconnect();
   
    response.clear();

    client.Connect(HTTP_PORT, "www.xserver.ru");
    response = client.SendHttpRequest("post", "/computer/servers/razn/28/", "1.1");

    std::cout << "Response 2: " << response << std::endl;

    client.Disconnect();
    return 0;
	
}

