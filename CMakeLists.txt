cmake_minimum_required(VERSION 3.17)
project(ClimateFinder)

set(CMAKE_CXX_STANDARD 14)

add_executable(ClimateFinder ClimateFinder.cpp LocData.cpp LocData.h)