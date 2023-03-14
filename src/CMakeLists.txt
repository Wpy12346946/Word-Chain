cmake_minimum_required(VERSION 3.24)
project(Wordlist)

set(CMAKE_CXX_STANDARD 14)

#add_library(core SHARED  core/Core.cpp core/Edge.cpp core/Graph.cpp)
#set_target_properties(core PROPERTIES PREFIX "")

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
link_directories(../bin)
add_executable(Wordlist Error.h run.cpp CommonLine.cpp)
target_link_libraries(Wordlist core.dll)