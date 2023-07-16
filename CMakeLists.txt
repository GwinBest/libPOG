cmake_minimum_required(VERSION 3.0)

project(libPOG)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

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
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # x64 Debug
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_BINARY_DIR}/lib/x64/debug)
        file(COPY
            dependencies/openssl/x64/lib/
            dependencies/openssl/x64/bin/
            DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
        )
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        # x86 Debug
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_BINARY_DIR}/lib/x86/debug)
        file(COPY
            dependencies/openssl/x86/lib/
            dependencies/openssl/x86/bin/
            DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
        )
    endif()
    
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # x64 Release
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR}/lib/x64/release)
        file(COPY
            dependencies/openssl/x64/lib/
            dependencies/openssl/x64/bin/
            DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}
        )
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        # x86 Release
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR}/lib/x86/release)
        file(COPY
            dependencies/openssl/x86/lib/
            dependencies/openssl/x86/bin/
            DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}
        )
    endif()
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


