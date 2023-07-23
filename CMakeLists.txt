cmake_minimum_required(VERSION 3.2)

set(TARGET CURVE_MY_CORNERS)

project(${TARGET} LANGUAGES CXX VERSION 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include/")
set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/src/")

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -O3 -pedantic-errors -fmax-errors=1)
else()
    message(FATAL_ERROR "curve-my-corners can only be built with GNU C++ currently")
endif()

target_link_libraries(${TARGET} sfml-graphics sfml-window sfml-system)