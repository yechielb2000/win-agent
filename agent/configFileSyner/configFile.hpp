//
// Created by yechi on 5/25/2025.
//

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP
#include <string>


struct ProcessInfo {
    std::string name;
    std::string path;
    std::string hash256;
};

struct configFile {

};

void loadConfigFile();

void parseConfigFileData();


#endif //CONFIGFILE_HPP
