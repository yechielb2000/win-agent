cmake_minimum_required(VERSION 3.31)
project(WinAgent)

set(CMAKE_CXX_STANDARD 20)

add_executable(WinAgent agent/agent.cpp
        agent/detectionMethods/detectionMethod.cpp
        agent/detectionMethods/detectionMethod.hpp
        agent/configFileSyner/configFile.cpp
        agent/configFileSyner/configFile.hpp
        agent/configFileSyner/syncer.cpp
        agent/configFileSyner/syncer.hpp
        agent/commonHeaders/processInfo.hpp
        agent/detectionMethods/fsRelated/basicComparison.cpp
        agent/detectionMethods/fsRelated/basicComparison.hpp
        agent/detectionMethods/fsRelated/fsDetectionMethod.cpp
        agent/detectionMethods/fsRelated/fsDetectionMethod.hpp
        agent/logger/logger.cpp
        agent/logger/logger.hpp
)

target_link_libraries(WinAgent bcrypt)

