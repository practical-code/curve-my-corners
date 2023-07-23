cmake_minimum_required(VERSION 3.2)

set(TARGET curve-my-corners)

project(${TARGET} LANGUAGES CXX VERSION 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

add_executable(
    ${TARGET}
    "${PROJECT_SOURCE_DIR}/src/FileOpenException.cpp"
    "${PROJECT_SOURCE_DIR}/src/FileReadException.cpp"
    "${PROJECT_SOURCE_DIR}/src/Serializer.cpp"
    "${PROJECT_SOURCE_DIR}/src/Entry.cpp"
)

target_include_directories(
    ${TARGET}
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/third_party/cxxopts/include/"
    "${PROJECT_SOURCE_DIR}/third_party/json/single_include/nlohmann"
)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -O3 -pedantic-errors -fmax-errors=1)
else()
    message(FATAL_ERROR "curve-my-corners can only be built with GNU C++ currently")
endif()

target_link_libraries(${TARGET} sfml-graphics sfml-window sfml-system)