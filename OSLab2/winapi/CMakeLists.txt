cmake_minimum_required(VERSION 3.15)
project(winapi)

set(CMAKE_CXX_STANDARD 98)

set(BOOST_ROOT "C:/dev/vcpkg/installed/x64-windows")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(winapi
        src/main.cpp
        src/ArrayThreads.cpp
)

add_executable(tests
        tests/tests.cpp
        src/ArrayThreads.cpp
)

target_include_directories(tests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(tests ${Boost_LIBRARIES})