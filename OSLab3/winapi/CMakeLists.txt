cmake_minimum_required(VERSION 3.10)
project(Lab3WinApi)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(Lab3WinApi src/main.cpp src/worker.cpp)

add_executable(Lab3Tests tests/tests.cpp src/worker.cpp)