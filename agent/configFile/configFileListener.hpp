#ifndef CONFIG_FILE_LISTENER_HPP
#define CONFIG_FILE_LISTENER_HPP
#include <windows.h>


HANDLE CreateFileChangeListener(const char *configFilePath);

#endif
