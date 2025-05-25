#include "configFileListener.hpp"

#include <stdexcept>
#include <windows.h>


HANDLE CreateFileChangeListener(const char *configFilePath) {
    const HANDLE changeHandle = FindFirstChangeNotificationA(
        configFilePath,
        FALSE,
        FILE_NOTIFY_CHANGE_LAST_WRITE
    );

    if (changeHandle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to create file change notification handle");
    }
    return changeHandle;
}
