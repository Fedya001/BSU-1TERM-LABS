cmake_minimum_required(VERSION 3.12)
project(BiDirectionalList)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(BiDirectionalList main.cpp testing_framework.cpp tests.cpp)