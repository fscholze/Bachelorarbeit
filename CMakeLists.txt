cmake_minimum_required(VERSION 3.7)
project(Programm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Converter.cpp Converter.h DEFINE.h DTW.cpp DTW.h)
add_executable(Programm ${SOURCE_FILES})