#include "DetectionMethod.hpp"

#include "../logger/logger.hpp"

DetectionMethod::DetectionMethod(const std::vector<ProcessInfo> &processList): processList(processList),
                                                                               logger_(get_logger()) {}

void DetectionMethod::warn(const ProcessInfo &processInfo) const {
    this->logger_->info("** Suspicious File Found: {}", processInfo.toString());

}
