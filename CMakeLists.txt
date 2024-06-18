cmake_minimum_required(VERSION 3.0)

project(libPOG)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdParty/openssl ssl)

include_directories(${CMAKE_BINARY_DIR}/ssl/include)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
file(COPY 
    src/HTTPClient.h
    src/HTTPSClient.h
    src/buffer.h
    ${CMAKE_BINARY_DIR}/ssl/include/openssl
    DESTINATION ${CMAKE_BINARY_DIR}/include/libPOG
)

add_library(libPOG
    src/HTTPClient.h
    src/HTTPClient.cpp
    src/HTTPSClient.h
    src/HTTPSClient.cpp
    src/StringUtils.h
    src/StringUtils.cpp
    src/buffer.h
    test/main.cpp
)

target_link_libraries(libPOG ssl)