#ifndef DETECTIONMETHOD_HPP
#define DETECTIONMETHOD_HPP
#include <vector>

#include "../commonHeaders/processInfo.hpp"

class DetectionMethod {
    std::vector<ProcessInfo> &processList;

public:
    DetectionMethod();

    explicit DetectionMethod(std::vector<ProcessInfo> &processList);

    void warn(const ProcessInfo &processInfo);
};


#endif
