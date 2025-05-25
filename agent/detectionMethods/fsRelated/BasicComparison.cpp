#include <windows.h>
#include <wincrypt.h>
#include "basicComparison.hpp"


void BasicComparison::runDetection() {
    for (ProcessInfo processInfo: this->processList) {
        std::string fileBuffer = this->loadFile(processInfo.path);
        if (isSHA256Matched(processInfo, fileBuffer)) {
            this->warn(processInfo);
        }
    }
}

bool BasicComparison::isSHA256Matched(const ProcessInfo &processInfo, std::string fileBuffer) {
    /*
    ok, I found that this is a legacy and there is a newer approach using BCryptCreateHash..
    I will use the new one once I completed the rest. since this is also something for now.
    */
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[32];
    DWORD hashLen = 32;
    BOOL result = FALSE;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return false;
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return false;
    }

    if (!CryptHashData(hHash, reinterpret_cast<BYTE *>(&fileBuffer), sizeof(fileBuffer), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return false;
    }

    result = memcmp(hash, processInfo.sha256.data(), 32) == 0;

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return result;
}
