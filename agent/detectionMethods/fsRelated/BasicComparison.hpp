#ifndef BASICCOMPARISON_HPP
#define BASICCOMPARISON_HPP

#include "fsDetectionMethod.hpp"
#include "../DetectionMethod.hpp"

class BasicComparison : public fsDetectionMethod {
public:
    explicit BasicComparison(const std::vector<ProcessInfo> &processList);

    bool isSHA256Matched(const ProcessInfo &processInfo, char fileBuffer);

    void runDetection();
};


#endif
