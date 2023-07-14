cmake_minimum_required(VERSION 3.0)
project(libPOG)

include_directories(dependencies/openssl/include)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(COPY 
    client/src/HTTP/HTTPClient.h
    client/src/HTTPS/HTTPSClient.h
    DESTINATION ${CMAKE_BINARY_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)

add_library(libPOG
    client/src/HTTP/HTTPClient.h
    client/src/HTTP/HTTPClient.cpp
    client/src/HTTPS/HTTPSClient.h
    client/src/HTTPS/HTTPSClient.cpp
    client/src/string.h
    client/src/string.cpp
    client/src/buffer.h
    client/src/clientStatus.h
)

target_link_libraries(libPOG
    ${CMAKE_BINARY_DIR}/dependencies/openssl/lib/libssl.lib
    ${CMAKE_BINARY_DIR}/dependencies/openssl/lib/libcrypto.lib
)
