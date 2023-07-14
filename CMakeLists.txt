cmake_minimum_required(VERSION 3.0)
project(libPOG)

include_directories(dependencies/openssl/include)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(COPY 
    client/src/HTTP/HTTPClient.h
    client/src/HTTPS/HTTPSClient.h
    DESTINATION ${CMAKE_BINARY_DIR}/include)


if(CMAKE_VS_PLATFORM_NAME STREQUAL "x86")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x86)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x86)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x86)
elseif(CMAKE_VS_PLATFORM_NAME STREQUAL "x64")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x64)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/x64)
endif()

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
