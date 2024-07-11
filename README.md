# libPOG
## Getting Started 
### First, download the latest version of [CMake](https://cmake.org/download/)
Then, clone a repository
This repo uses gitmodules, so clone it recursive:
```
git clone --recursive https://github.com/GwinBest/libPOG
```
Make a `build` folder 
```
cd libPOG
mkdir build
cd build
```
Then, run Cmake
```
cmake ..
```
Build the project
```
cmake --build .
```
At this point, you will have a static library at `lib\libPOG.(your OS static library extension)`. 
All you have to do now - is to include `include\` folder to your project, link the library and add `libcrypto.lib`, `libssl.lib` and `libPOG.lib` to additional dependencies.
## Usage example 
``` c++
#include <HTTPClient.h>
#include <HTTPSClient.h>

int main()
{
//---https client---//

    Net::HTTPSClient httpClient;
    httpClient.HTTPSConnect(HTTPS_PORT, "en.wikipedia.org");
    std::string response = httpClient.SendHttpsRequest("get", "/wiki/Manchester_United_F.C.", "1.1");

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

```
