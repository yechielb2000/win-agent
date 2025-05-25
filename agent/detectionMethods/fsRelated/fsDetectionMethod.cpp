#include "fsDetectionMethod.hpp"

#include <windows.h>

std::string fsDetectionMethod::loadFile(const std::string &filepath) {
    // should I do shadow copy to read it?
    char bufferToRead[MAX_PATH];

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

    BOOL readResult = ReadFile(
        hFile,
        bufferToRead,
        sizeof(bufferToRead),
        0,
        NULL
    );

    if (!readResult) {
        throw "Unable to read file: " + filepath;
    }
    CloseHandle(hFile);
    return std::string(bufferToRead);
}
