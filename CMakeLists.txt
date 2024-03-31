cmake_minimum_required(VERSION 3.28)
project(epoll_tutorial)

find_package(CURL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(epoll_tutorial src/epoll/main.cpp)

add_executable(epoll_curl_client src/client/curl/main.cpp)
target_link_libraries(epoll_curl_client ${CURL_LIBRARIES})