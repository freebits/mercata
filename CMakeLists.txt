cmake_minimum_required(VERSION 3.16)
project(mercata LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mercata src/main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(mercata PRIVATE Threads::Threads)
