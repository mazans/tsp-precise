cmake_minimum_required(VERSION 3.9)
project(PEA_ETAP1)

set(CMAKE_CXX_STANDARD 11)

add_executable(PEA_ETAP1 main.cpp Graph.h Graph.cpp TSPResolver.cpp TSPResolver.h DynamicMemoryTable.cpp DynamicMemoryTable.h)