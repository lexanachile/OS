cmake_minimum_required(VERSION 3.14)
project(LabWork2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_library(core src/core.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(factorial src/factorial.cpp)
target_link_libraries(factorial PUBLIC core)
target_include_directories(factorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE core factorial)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(GTest)
if(NOT GTest_FOUND)
    message(WARNING "GTest not found. Downloading from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_executable(test_factorial tests/test_factorial.cpp)
set_target_properties(test_factorial PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(test_factorial PRIVATE core factorial GTest::gtest_main)
add_test(NAME test_factorial COMMAND ${CMAKE_SOURCE_DIR}/bin/test_factorial)

add_executable(test_duplicate_remover tests/test_duplicate_remover.cpp)
set_target_properties(test_duplicate_remover PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(test_duplicate_remover PRIVATE core GTest::gtest_main)
add_test(NAME test_duplicate_remover COMMAND ${CMAKE_SOURCE_DIR}/bin/test_duplicate_remover)

add_executable(test_linked_list tests/test_linked_list.cpp)
set_target_properties(test_linked_list PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(test_linked_list PRIVATE core GTest::gtest_main)
add_test(NAME test_linked_list COMMAND ${CMAKE_SOURCE_DIR}/bin/test_linked_list)

add_executable(test_integration tests/test_integration.cpp)
set_target_properties(test_integration PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
target_link_libraries(test_integration PRIVATE core factorial GTest::gtest_main)
add_test(NAME test_integration COMMAND ${CMAKE_SOURCE_DIR}/bin/test_integration)
