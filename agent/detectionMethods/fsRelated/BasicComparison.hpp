#ifndef BASICCOMPARISON_HPP
#define BASICCOMPARISON_HPP

#include "../DetectionMethod.hpp"

class BasicComparison : DetectionMethod {
public:
    explicit BasicComparison(const std::vector<ProcessInfo> &processList);

    bool isHashMatched(const ProcessInfo &processInfo, char fileBuffer);

    void runDetection();
};


#endif
