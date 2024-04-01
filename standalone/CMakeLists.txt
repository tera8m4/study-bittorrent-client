cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(BittorrentStandalone LANGUAGES CXX)


# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)



CPMAddPackage(NAME Bittorrent SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../bittorrent)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "bittorrent")

target_link_libraries(${PROJECT_NAME} Bittorrent::Bittorrent)


