cmake_minimum_required(VERSION 2.6)
project(moa)

add_executable(moa moa.cpp)

install(TARGETS moa RUNTIME DESTINATION bin)
