cmake_minimum_required(VERSION 3.31)
project(WinAgent)

set(CMAKE_CXX_STANDARD 20)

add_executable(WinAgent agent/agent.cpp
        agent/detectionMethods/detectionMethod.cpp
        agent/detectionMethods/detectionMethod.hpp
)
