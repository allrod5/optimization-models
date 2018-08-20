cmake_minimum_required(VERSION 3.12)
project(optimization_models)

set(CMAKE_CXX_STANDARD 17)

add_executable(optimization_models main.cpp branches.cpp branches.h Board.cpp Board.h easyscip/easyscip.h constraint/constraint.h)

target_link_libraries(optimization_models PUBLIC m)
target_link_libraries(optimization_models PUBLIC scip)