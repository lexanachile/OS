cmake_minimum_required(VERSION 3.14)
project(OSLab1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_executable(Reporter reporter.cpp)
add_executable(Creator creator.cpp)
add_executable(main_app main.cpp)

set_target_properties(Reporter Creator main_app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(GTest)
if(NOT GTest_FOUND)
    message(WARNING "GTest not found. Tests will be disabled.")
else()
    enable_testing()

    add_executable(test_creator test_creator.cpp)
    set_target_properties(test_creator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    target_link_libraries(test_creator GTest::gtest_main)
    add_test(NAME test_creator COMMAND ${CMAKE_SOURCE_DIR}/bin/test_creator)

    add_executable(test_reporter test_reporter.cpp)
    set_target_properties(test_reporter PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    target_link_libraries(test_reporter GTest::gtest_main)
    add_test(NAME test_reporter COMMAND ${CMAKE_SOURCE_DIR}/bin/test_reporter)

    add_executable(test_main test_main.cpp)
    set_target_properties(test_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    target_link_libraries(test_main GTest::gtest_main)
    add_test(NAME test_main COMMAND ${CMAKE_SOURCE_DIR}/bin/test_main)
endif()