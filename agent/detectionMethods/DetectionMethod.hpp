#ifndef DETECTIONMETHOD_HPP
#define DETECTIONMETHOD_HPP
#include <memory>
#include <vector>

#include "../commonHeaders/processInfo.hpp"
#include "spdlog/logger.h"


class DetectionMethod {
protected:
    std::vector<ProcessInfo> processList;
    std::shared_ptr<spdlog::logger> logger_;

public:
    explicit DetectionMethod(const std::vector<ProcessInfo> &processList);

    void warn(const ProcessInfo &processInfo);
};


#endif
