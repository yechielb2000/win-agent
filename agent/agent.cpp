#include <iostream>
#include <windows.h>
#include <vector>
#include "commonHeaders/processInfo.hpp"
#include "configFileSyner/configFile.hpp"
#include "detectionMethods/fsRelated/BasicComparison.hpp"
#include "logger/logger.hpp"

int main() {
    setup_logger();
    const std::vector<ProcessInfo> processInfos = loadConfigFile("..\\tests\\output.csv");
    auto bc = BasicComparison(processInfos);
    bc.runDetection();
    return 0;
}
