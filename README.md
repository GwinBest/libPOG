# libPOG
## Getting Started 
### First, download the latest version of [CMake](https://cmake.org/download/)
Then, clone a repository
```
git clone https://github.com/GwinBest/libPOG
```
Make a `builds-x64` folder 
```
cd libPOG
mkdir builds-x64
cd builds-x64
```
Then, run Cmake
```
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR_PLATFORM=x64 ..
```
Build the project
```
cmake --build . --config Debug
```
At this point, you will have a static library at `lib\x64\${Build Configuration}\libPOG.(your OS static library extension)`. 
All you have to do now - is to include `include\` folder to your project, link the library, add `libcrypto.lib`, `libssl.lib` and `libPOG.lib` to additional dependencies
and add `libcrypto-3-x64.dll` and `libssl-3-x64.dll` to your executable.
#### If you wnat to create for `x86` you should create a new folder e.g. `builds-x86` and set `-DCMAKE_GENERATOR_PLATFORM=Win32`
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
