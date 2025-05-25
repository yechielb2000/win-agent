#include "fsDetectionMethod.hpp"

#include <windows.h>

HANDLE fsDetectionMethod::loadFile(const std::string &filepath) {
    // should I do shadow copy to read it?
    const HANDLE hFile = CreateFile(
        filepath.c_str(),
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        throw "Unable to open file: " + filepath;
    }
    return hFile;
}
