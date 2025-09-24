cmake_minimum_required(VERSION 3.14)
project(OSLab1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(INSTALL_GTEST OFF)
set(BUILD_GMOCK OFF)

FetchContent_MakeAvailable(googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(Reporter reporter.cpp)
add_executable(Creator creator.cpp)
add_executable(main_app main.cpp)

add_executable(test_creator test_creator.cpp)
target_link_libraries(test_creator gtest_main)

add_executable(test_reporter test_reporter.cpp)
target_link_libraries(test_reporter gtest_main)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main gtest_main)


enable_testing()


add_test(NAME test_creator COMMAND test_creator)
add_test(NAME test_reporter COMMAND test_reporter)
add_test(NAME test_main COMMAND test_main)