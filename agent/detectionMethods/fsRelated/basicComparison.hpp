#ifndef BASICCOMPARISON_HPP
#define BASICCOMPARISON_HPP

#include "../DetectionMethod.hpp"

class basicComparison : DetectionMethod {
public:
    bool isNameMatched(const ProcessInfo &processInfo);

    bool isPathMatched(const ProcessInfo &processInfo);

    bool isHashMatched(const ProcessInfo &processInfo);
};


#endif
