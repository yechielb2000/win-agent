#ifndef FSDETECTIONMETHOD_HPP
#define FSDETECTIONMETHOD_HPP

#include <bemapiset.h>

#include "../DetectionMethod.hpp""

class fsDetectionMethod : public DetectionMethod {
public:
    static char loadFile(const std::string &filepath);
};


#endif
