#include <iostream>
#include <windows.h>
#include <vector>
#include <mutex>
#include "configFile/configFile.hpp"
#include "detectionMethods/fsRelated/BasicComparison.hpp"
#include "logger/logger.hpp"
#include "agent.hpp"


int main() {
#if DEBUG
    constexpr SERVICE_TABLE_ENTRY ServiceTable[] = {
        {const_cast<LPSTR>(SERVICE_NAME), static_cast<LPSERVICE_MAIN_FUNCTION>(WinAgentService)},
        {NULL, NULL}
    };
    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
        return GetLastError();
    }
#endif
    WinAgentThread(LPVOID());
    return 0;
}

DWORD WinAgentThread(LPVOID lpParam) {
    try {
        setup_logger();
    } catch (const std::exception &e) {
        OutputDebugStringA("Failed to load logger");
        return ERROR_LOG_CLIENT_NOT_REGISTERED;
    }

    updateProcessList();

    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0) {
        auto bc = BasicComparison(g_ProcessInfos);
        bc.runDetection();
    }
    return ERROR_SUCCESS;
}

DWORD WINAPI ConfigFileListenerThread(LPVOID lpParam) {
    const HANDLE changeHandle = ConfigFileChangeListener(CONFIG_FILE_PATH.c_str());

    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0) {
        if (WaitForSingleObject(changeHandle, 1000) == WAIT_OBJECT_0) {
            try {
                updateProcessList();

                if (!FindNextChangeNotification(changeHandle)) {
                    OutputDebugStringA("Failed to reset change notification");
                    break;
                }
            } catch ([[maybe_unused]] const std::exception &e) {
                OutputDebugStringA("Failed to process file change");
            }
        }
    }

    FindCloseChangeNotification(changeHandle);
    return ERROR_SUCCESS;
}


HANDLE ConfigFileChangeListener(const char *configFilePath) {
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


VOID WINAPI WinAgentService(DWORD argc, LPTSTR *argv) {
    const auto statusHandle = RegisterServiceCtrlHandlerA(SERVICE_NAME, ServiceCtrlHandler);
    if (statusHandle == NULL) {
        return;
    }

    g_StatusHandle = statusHandle;

    ZeroMemory(&g_ServiceStatus, sizeof (g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwControlsAccepted = 0;
    updateServiceStatus(SERVICE_START_PENDING, 0, 1);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        g_ServiceStatus.dwControlsAccepted = 0;
        updateServiceStatus(SERVICE_STOPPED, GetLastError(), 1);
        return;
    }

    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    updateServiceStatus(SERVICE_RUNNING, 0, 0);


    const HANDLE winAgentThreadHandle = CreateThread(
        NULL,
        0,
        WinAgentThread,
        NULL,
        0,
        NULL
    );

    const HANDLE fileListenerThreadHandle = CreateThread(
        NULL,
        0,
        ConfigFileListenerThread,
        NULL,
        0,
        NULL
    );

    const HANDLE handles[] = {fileListenerThreadHandle, winAgentThreadHandle};

    WaitForMultipleObjects(2, handles, TRUE, INFINITE);

    CloseHandle(g_ServiceStopEvent);
    CloseHandle(winAgentThreadHandle);
    CloseHandle(fileListenerThreadHandle);

    g_ServiceStatus.dwControlsAccepted = 0;
    updateServiceStatus(SERVICE_STOPPED, 0, 0);
}


VOID WINAPI ServiceCtrlHandler(const DWORD ctrlCode) {
    switch (ctrlCode) {
        case SERVICE_CONTROL_STOP:

            if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
                break;
            g_ServiceStatus.dwControlsAccepted = 0;
            g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            g_ServiceStatus.dwWin32ExitCode = 0;
            g_ServiceStatus.dwCheckPoint = 4;

            if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE) {
                OutputDebugStringA("Failed to update service status");
            }

            SetEvent(g_ServiceStopEvent);
            break;

        default:
            break;
    }
}


long updateProcessList() {
    try {
        auto processList = loadConfigFile(CONFIG_FILE_PATH);
        std::lock_guard lock(g_ProcessInfosMutex);
        g_ProcessInfos = std::move(processList);
    } catch ([[maybe_unused]] const std::exception &e) {
        OutputDebugStringA("Failed initial config load");
        return ERROR_FILE_NOT_FOUND;
    }
    return 0;
}

void updateServiceStatus(const DWORD dwCurrentState, const DWORD dwWin32ExitCode, const DWORD dwCheckPoint) {
    g_ServiceStatus.dwCurrentState = dwCurrentState;
    g_ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
    g_ServiceStatus.dwCheckPoint = dwCheckPoint;
    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE) {
        OutputDebugStringA("Failed to update service status");
    }
}
