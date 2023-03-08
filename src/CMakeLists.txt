cmake_minimum_required(VERSION 3.24)
project(src)

set(CMAKE_CXX_STANDARD 14)

link_directories(bin)
add_executable(src CMakeProject2.cpp)
target_link_libraries(src core.dll)
