cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME skeleton_smash)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES *.cpp *.h)

add_executable(skeleton_smash ${SOURCES})

include(CTest)

add_subdirectory(tests)