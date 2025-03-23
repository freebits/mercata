cmake_minimum_required(VERSION 3.16)
project(mercata LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(mercata_server src/mercata_server.cpp)
target_link_libraries(mercata_server PRIVATE Threads::Threads)

add_executable(mercata_client src/mercata_client.cpp)
target_link_libraries(mercata_client PRIVATE Threads::Threads)
