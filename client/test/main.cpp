#include "../src/client.h"

#include <locale>


int main()
{
    setlocale(LC_ALL, "ru");
    Net::Client client;
    client.Connect(HTTP_PORT, "www.xserver.ru");
    std::string response = client.SendHttpRequest("GET", "/computer/servers/razn/29/", "1.1");

    std::cout << "Response 1: " << response << std::endl;
    client.Disconnect();
   
    response.clear();

    client.Connect(HTTP_PORT, "www.xserver.ru");
    response = client.SendHttpRequest("post", "/computer/servers/razn/28/", "1.1");

    std::cout << "Response 2: " << response << std::endl;

    client.Disconnect();
    return 0;
	
}

