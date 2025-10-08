cmake_minimum_required(VERSION 3.15)
project(boost)

set(CMAKE_CXX_STANDARD 11)

# Указываем путь к vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Ищем все необходимые компоненты Boost
find_package(Boost REQUIRED COMPONENTS thread chrono system unit_test_framework)

add_executable(boost
        src/main.cpp
        src/ArrayThreads.cpp
        src/ArrayThreads.h
        src/main.cpp
        src/main.cpp
)

add_executable(tests
        tests/tests.cpp
        src/ArrayThreads.cpp
)

# Линкуем все необходимые библиотеки Boost
target_link_libraries(boost
        Boost::thread
        Boost::chrono
        Boost::system
)

target_link_libraries(tests
        Boost::thread
        Boost::chrono
        Boost::system
        Boost::unit_test_framework
)