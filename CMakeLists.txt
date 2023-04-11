cmake_minimum_required(VERSION 3.10)
project(skeleton_smash)

set(CMAKE_CXX_STANDARD 14)

file(GLOB source *.cpp *.h)

add_executable(skeleton_smash ${source})