cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(BuildAll LANGUAGES CXX)

enable_testing()
include(cmake/tools.cmake)
# needed to generate test target





# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/bittorrent ${CMAKE_BINARY_DIR}/bittorrent)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/standalone ${CMAKE_BINARY_DIR}/standalone)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test ${CMAKE_BINARY_DIR}/test)

