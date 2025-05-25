#include "DetectionMethod.hpp"

DetectionMethod::DetectionMethod(const std::vector<ProcessInfo> &processList): processList() {
    this->processList = processList;
}

void DetectionMethod::warn(const ProcessInfo &processInfo) {
    // Log out warning for this file.
}
