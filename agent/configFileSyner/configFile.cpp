#include "configFile.hpp"

#include <fstream>
#include <vector>
#include <sstream>
#include "../commonHeaders/processInfo.hpp"

// TODO: maybe split to load file and parse file.
std::vector<ProcessInfo> loadConfigFile(const std::string &filename) {
    std::vector<ProcessInfo> processList;

    if (std::ifstream file(filename); file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            if (row.size() == 3) {
                ProcessInfo processInfo;
                processInfo.name = row[0];
                processInfo.path = row[1];
                processInfo.sha256 = row[2];
                processList.push_back(processInfo);
            } else {
                throw "Invalid row size: " + line;
            }
        }
        file.close();
    } else {
        throw "Unable to open file: " + filename;
    }

    return processList;
}
