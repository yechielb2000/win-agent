#ifndef PROCESSINFO_HPP
#define PROCESSINFO_HPP

#pragma once
#include <iostream>


struct ProcessInfo {
    std::string name;
    std::string path;
    std::string sha256;

    [[nodiscard]] std::string toString() const {
        return "name: " + name + " | " "path: " + path + " | " "sha256: " + sha256;
    }
};


#endif
