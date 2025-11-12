cmake_minimum_required(VERSION 3.15)
project(erosion-simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})