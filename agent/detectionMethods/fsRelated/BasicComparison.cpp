#include <windows.h>
#include <bcrypt.h>
#include <sstream>
#include <iomanip>
#include "basicComparison.hpp"


#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

void BasicComparison::runDetection() {
    for (ProcessInfo processInfo: this->processList) {
        std::string fileBuffer = this->loadFile(processInfo.path);
        if (isSHA256Matched(processInfo, fileBuffer)) {
            this->warn(processInfo);
        }
    }
}

bool BasicComparison::isSHA256Matched(const ProcessInfo &processInfo, const std::string &fileBuffer) {
    BCRYPT_ALG_HANDLE hAlg;
    BCRYPT_HASH_HANDLE hHash = NULL;
    DWORD cbData = 0, cbHash = 0, cbHashObject = 0;
    PBYTE pbHashObject = NULL, pbHash = NULL;

    NTSTATUS status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0);
    if (!NT_SUCCESS(status)) {
        return false;
    }
    status = BCryptGetProperty(
        hAlg,
        BCRYPT_OBJECT_LENGTH,
        reinterpret_cast<PBYTE>(&cbHashObject), sizeof(DWORD),
        &cbData,
        0
    );
    if (!NT_SUCCESS(status)) {
        return false;
    }
    pbHashObject = static_cast<PBYTE>(HeapAlloc(GetProcessHeap(), 0, cbHashObject));
    if (NULL == pbHashObject) {
        return false;
    }
    status = BCryptGetProperty(
        hAlg,
        BCRYPT_HASH_LENGTH,
        reinterpret_cast<PBYTE>(&cbHash),
        sizeof(DWORD),
        &cbData,
        0
    );
    if (!NT_SUCCESS(status)) {
        return false;
    }
    pbHash = static_cast<PBYTE>(HeapAlloc(GetProcessHeap(), 0, cbHash));
    if (NULL == pbHash) {
        return false;
    }

    status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, NULL, 0, 0);
    if (!NT_SUCCESS(status)) {
        return false;
    }

    status = BCryptHashData(hHash, const_cast<PUCHAR>(reinterpret_cast<const BYTE *>(fileBuffer.c_str())),
                            fileBuffer.size(), 0);
    if (!NT_SUCCESS(status)) {
        return false;
    }

    status = BCryptFinishHash(hHash, pbHash, cbHash, 0);
    if (!NT_SUCCESS(status)) {
        return false;
    }

    BCryptCloseAlgorithmProvider(hAlg, 0);
    std::stringstream hashStream;
    for (DWORD i = 0; i < cbHash; i++) {
        hashStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pbHash[i]);
    }
    return hashStream.str() == processInfo.sha256;
}
