# CMakeList.txt: CMakeProject2 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable(CMakeProject2 "CMakeProject2.cpp" "CMakeProject2.h" "Edge.cpp" "Edge.h" "Graph.h" "Graph.cpp" "Core.h" "Core.cpp")
# TODO: 如有需要，请添加测试并安装目标。
