#ifndef FSDETECTIONMETHOD_HPP
#define FSDETECTIONMETHOD_HPP

#include <bemapiset.h>

#include "../DetectionMethod.hpp""

class fsDetectionMethod : DetectionMethod {
public:
    static HANDLE loadFile(const std::string &filepath);
};


#endif
