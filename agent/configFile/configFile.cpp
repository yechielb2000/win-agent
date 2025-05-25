#include "configFile.hpp"

#include <fstream>
#include <vector>
#include <sstream>
#include "../commonHeaders/processInfo.hpp"
#include "../logger/logger.hpp"
#include <shlobj.h>


std::vector<ProcessInfo> loadConfigFile(const std::string &filename) {
    auto logger = get_logger();
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
                logger->error("Invalid row size: " + line);
            }
        }
        file.close();
    } else {
        logger->error("Unable to open file: " + filename);
    }

    return processList;
}

std::string GetConfigFilePath() {
    WCHAR desktopPath[MAX_PATH];

    if (FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
        NULL, SHGFP_TYPE_CURRENT, desktopPath))) {
        return std::string();
    }

    std::wstring wFullPath = desktopPath;
    if (wFullPath.back() != L'\\') {
        wFullPath += L'\\';
    }
    wFullPath += L"win_agent_config_file.csv";

    const int size_needed = WideCharToMultiByte(
        CP_UTF8,
        0,
        wFullPath.c_str(),
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );
    if (size_needed <= 0) {
        return std::string();
    }

    std::string absolutePath(size_needed, 0);
    if (WideCharToMultiByte(
            CP_UTF8,
            0,
            wFullPath.c_str(),
            -1,
            &absolutePath[0],
            size_needed,
            nullptr,
            nullptr
        ) <= 0) {
        return std::string();
    }

    absolutePath.resize(strlen(absolutePath.c_str()));
    return absolutePath;
}
