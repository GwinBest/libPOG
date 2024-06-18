cmake_minimum_required(VERSION 3.0)
project(libPOG)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdParty/openssl ssl)

add_library(libPOG
    src/HTTPClient.h
    src/HTTPClient.cpp
    src/HTTPSClient.h
    src/HTTPSClient.cpp
    src/StringUtils.h
    src/StringUtils.cpp
    src/buffer.h
)

target_include_directories(libPOG PUBLIC ${CMAKE_BINARY_DIR}/ssl/include)
target_link_libraries(libPOG PUBLIC ssl)

set_target_properties(ssl crypto libPOG PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Copy headers
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/libPOG)
file(COPY 
    src/HTTPClient.h
    src/HTTPSClient.h
    src/buffer.h
    ${CMAKE_BINARY_DIR}/ssl/include/openssl
    DESTINATION ${CMAKE_BINARY_DIR}/include/libPOG
)
