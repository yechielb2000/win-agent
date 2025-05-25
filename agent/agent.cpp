#include <iostream>
#include <windows.h>
#include <vector>
#include "commonHeaders/processInfo.hpp"
#include "configFileSyner/configFile.hpp"
#include "detectionMethods/fsRelated/BasicComparison.hpp"

int main() {
    const std::vector<ProcessInfo> processInfos = loadConfigFile("..\\tests\\output.csv");
    auto bc = BasicComparison(processInfos);
    bc.runDetection();
    return 0;
}
