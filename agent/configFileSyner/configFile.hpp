#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP
#include <string>
#include <vector>
#include "../commonHeaders/processInfo.hpp"

struct configFile {
};

std::vector<ProcessInfo> loadConfigFile(const std::string &filename);


#endif
