cmake_minimum_required(VERSION 3.5)

project(ABSCtl LANGUAGES CXX VERSION 0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(src)

add_executable(absctl ${MAIN_SRC})
target_include_directories(absctl PUBLIC ${INCLUDE_DIR})
target_include_directories(absctl PUBLIC dist/eternal/include)
target_link_libraries(absctl curl)

add_compile_options(-pg)