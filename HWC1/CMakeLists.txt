cmake_minimum_required(VERSION 3.6)
project(HWC1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c msg.h msg.c buffer.c buffer.h)
add_executable(HWC1 ${SOURCE_FILES})