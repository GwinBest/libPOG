#include "client.h"
#include <locale>

const char* host = "www.xserver.ru";
const char* uri = "/computer/servers/razn/29/";

int main()
{
	setlocale(LC_ALL, "rus");
	Net::Client client;
	client.Connect(HTTP_PORT, "www.xserver.ru");
	std::string response = client.SendHttpRequest("GET", "", "1.1");
	client.Disconnect();
	


	getc(stdin);

	return 0;
}

