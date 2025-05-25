#ifndef BASICCOMPARISON_HPP
#define BASICCOMPARISON_HPP

#include "../DetectionMethod.hpp"

class BasicComparison : DetectionMethod {
public:
    bool isHashMatched(const ProcessInfo &processInfo);
};


#endif
