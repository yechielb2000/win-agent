#ifndef FSDETECTIONMETHOD_HPP
#define FSDETECTIONMETHOD_HPP

#include "../DetectionMethod.hpp"
#include "spdlog/spdlog.h"


class fsDetectionMethod : public DetectionMethod {
public:
    using DetectionMethod::DetectionMethod;

    static std::string loadFile(const std::string &filepath);
};


#endif
