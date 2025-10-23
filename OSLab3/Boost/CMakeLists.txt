cmake_minimum_required(VERSION 3.10)
project(Lab3Boost)

set(CMAKE_PREFIX_PATH
        "C:/Users/yaras/.vcpkg-clion/vcpkg/installed/x64-mingw-static/share/boost"
        ${CMAKE_PREFIX_PATH}
)

cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.88 REQUIRED COMPONENTS thread system chrono)

add_executable(Lab3Boost src/main.cpp src/worker.cpp)
target_link_libraries(Lab3Boost ${Boost_LIBRARIES})

add_executable(Lab3Tests tests/tests.cpp src/worker.cpp)

target_link_libraries(Lab3Boost
        Boost::thread
        Boost::system
        Boost::chrono
)