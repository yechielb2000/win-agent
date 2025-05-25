#ifndef FSDETECTIONMETHOD_HPP
#define FSDETECTIONMETHOD_HPP

#include "../DetectionMethod.hpp"

class fsDetectionMethod : public DetectionMethod {
public:
    using DetectionMethod::DetectionMethod;

    static std::string loadFile(const std::string &filepath);
};


#endif
