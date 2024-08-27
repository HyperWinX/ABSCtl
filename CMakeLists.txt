cmake_minimum_required(VERSION 3.28)

project(ABSCtl LANGUAGES CXX VERSION 0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)

add_executable(absctl ${MAIN_SRC})
target_include_directories(absctl PUBLIC ${INCLUDE_DIR})
target_link_libraries(absctl curl)