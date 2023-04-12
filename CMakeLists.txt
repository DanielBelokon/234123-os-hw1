cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME skeleton_smash)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES *.cpp *.h)

add_executable(skeleton_smash ${SOURCES})

include(CTest)

add_subdirectory(tests)


add_custom_target(memcheck
    DEPENDS ${PROJECT_NAME}_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --test-action memcheck --parallel 8
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)