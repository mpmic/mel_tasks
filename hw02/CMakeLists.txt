cmake_minimum_required(VERSION 3.14)
project(hw02)

set(CMAKE_CXX_STANDARD 20)

set(LINK_LIBRARY_NAME hw02)
set(EXECUTABLE_NAME runhw02)

add_library(${LINK_LIBRARY_NAME} combinatorics.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXECUTABLE_NAME} run.cpp)

target_link_libraries(${EXECUTABLE_NAME} ${LINK_LIBRARY_NAME})
