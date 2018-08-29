cmake_minimum_required(VERSION 3.12)
project(optimization_models)

set(CMAKE_CXX_STANDARD 17)

add_executable(optimization_models main.cpp branches/branches.cpp branches/branches.h branches/Board.cpp branches/Board.h easyscip/easyscip.h constraint/constraint.h agents/agents.cpp agents/agents.h agents/Context.cpp agents/Context.h agents/Agent.cpp agents/Agent.h agents/Visit.cpp agents/Visit.h)

target_link_libraries(optimization_models PUBLIC m)
target_link_libraries(optimization_models PUBLIC scip)