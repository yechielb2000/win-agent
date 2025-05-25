#include "DetectionMethod.hpp"

#include "../logger/logger.hpp"

DetectionMethod::DetectionMethod(const std::vector<ProcessInfo> &processList): processList(processList) {
    this->processList = processList;
    this->logger_ = get_logger();
}

void DetectionMethod::warn(const ProcessInfo &processInfo) {
    this->logger_->warn("** Suspicious File Found {}", processInfo.toString());
}
