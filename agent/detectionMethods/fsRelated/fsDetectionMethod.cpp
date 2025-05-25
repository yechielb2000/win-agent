#include "fsDetectionMethod.hpp"

#include <windows.h>

std::string fsDetectionMethod::loadFile(const std::string &filepath) {
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

    LARGE_INTEGER fileSize;
    GetFileSizeEx(hFile, &fileSize);

    std::vector<char> fileBuffer(fileSize.QuadPart);
    DWORD bytesRead;

    const BOOL readResult = ReadFile(
        hFile,
        fileBuffer.data(),
        fileSize.QuadPart,
        &bytesRead,
        NULL
    );

    if (!readResult) {
        throw "Unable to read file: " + filepath;
    }
    CloseHandle(hFile);
    return std::string(fileBuffer.data(), bytesRead);
}
