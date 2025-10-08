# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(boost)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(boost
        src/main.cpp
        src/ArrayThreads.cpp
)

add_executable(tests
        tests/tests.cpp
        src/ArrayThreads.cpp
)

target_link_libraries(tests Boost::unit_test_framework)