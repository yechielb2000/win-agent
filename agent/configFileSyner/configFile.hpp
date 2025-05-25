#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP
#include <string>
#include <vector>


struct configFile {
};

std::vector<std::vector<std::string> > loadConfigFile(const std::string &filename);

void parseConfigFileData();


#endif
