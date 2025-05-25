#include "configFile.hpp"

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>


std::vector<std::vector<std::string> > loadConfigFile(const std::string &filename) {
    std::vector<std::vector<std::string> > data;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        file.close();
    } else {
        throw "Unable to open file: " + filename;
    }

    return data;
}
