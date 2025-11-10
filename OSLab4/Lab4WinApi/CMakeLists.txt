cmake_minimum_required(VERSION 3.10)
project(Lab4WinApi)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

set(COMMON_SOURCES src/utils.cpp src/queue_ops.cpp)

add_executable(Receiver src/receiver.cpp ${COMMON_SOURCES})
add_executable(Sender src/sender.cpp ${COMMON_SOURCES})

enable_testing()
add_executable(QueueTests tests/test_queue.cpp ${COMMON_SOURCES})
add_test(NAME QueueTests COMMAND QueueTests)