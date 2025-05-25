#ifndef DETECTIONMETHOD_HPP
#define DETECTIONMETHOD_HPP
#include <vector>

#include "../common/processInfo.hpp"

class DetectionMethod {
    std::vector<ProcessInfo> &processList;

public:
    DetectionMethod();

    explicit DetectionMethod(std::vector<ProcessInfo> &processList);
};


#endif
