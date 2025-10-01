cmake_minimum_required(VERSION 3.20)
project(winapi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH
        "C:/dev/vcpkg/installed/x64-windows/share/boost"
        ${CMAKE_PREFIX_PATH}
)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost CONFIG REQUIRED
        COMPONENTS thread chrono system unit_test_framework
)

add_executable(main
        src/main.cpp
        src/ArrayThreads.cpp
)
target_include_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(main PRIVATE
        Boost::thread
        Boost::chrono
        Boost::system
)

add_executable(tests
        tests/tests.cpp
        src/ArrayThreads.cpp
)
target_include_directories(tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(tests PRIVATE
        Boost::thread
        Boost::chrono
        Boost::system
        Boost::unit_test_framework
)

enable_testing()
add_test(NAME ArrayThreadsTest COMMAND tests)
