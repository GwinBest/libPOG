cmake_minimum_required(VERSION 3.0)
project(libPOG)

include_directories(dependencies/openssl/include)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(COPY 
    client/src/HTTPClient.h
    client/src/HTTPSClient.h
    client/src/StringUtils.h
    client/src/buffer.h
    client/src/clientStatus.h
    dependencies/openssl/include/openssl
    DESTINATION ${CMAKE_BINARY_DIR}/include)

# CMAKE_BUILD_TYPE == DEBUG by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_BINARY_DIR}/lib/x64/debug)
    file(COPY
        dependencies/openssl/x64/lib/
        DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
    )
    file(COPY
        dependencies/openssl/x64/lib/
        DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR}/lib/x64/release)
    file(COPY
        dependencies/openssl/x64/lib/
        DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}
    )
    file(COPY
        dependencies/openssl/x64/lib/
        DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}
    )
endif()

add_library(libPOG
    client/src/HTTPClient.h
    client/src/HTTPClient.cpp
    client/src/HTTPSClient.h
    client/src/HTTPSClient.cpp
    client/src/StringUtils.h
    client/src/StringUtils.cpp
    client/src/buffer.h
    client/src/clientStatus.h
)

target_link_libraries(libPOG
    ${CMAKE_BINARY_DIR}/dependencies/openssl/x64/lib/libcrypto.lib
    ${CMAKE_BINARY_DIR}/dependencies/openssl/x64/lib/libssl.lib
)
