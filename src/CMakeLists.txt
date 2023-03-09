cmake_minimum_required(VERSION 3.24)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_library(core SHARED  core/Core.cpp core/Edge.cpp core/Graph.cpp)
set_target_properties(core PROPERTIES PREFIX "")
#SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#link_directories(../bin)
#add_executable(src CMakeProject2.cpp)
#target_link_libraries(src core.dll)