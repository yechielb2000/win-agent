#ifndef BASICCOMPARISON_HPP
#define BASICCOMPARISON_HPP

#include "fsDetectionMethod.hpp"
#include "../DetectionMethod.hpp"

class BasicComparison : public fsDetectionMethod {
public:
    using fsDetectionMethod::fsDetectionMethod;

    bool isSHA256Matched(const ProcessInfo &processInfo, const std::string &fileBuffer);

    void runDetection();
};


#endif
