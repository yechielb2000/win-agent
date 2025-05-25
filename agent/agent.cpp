#include <iostream>
#include <windows.h>
#include <vector>
#include "commonHeaders/processInfo.hpp"
#include "configFile/configFile.hpp"
#include "detectionMethods/fsRelated/BasicComparison.hpp"
#include "logger/logger.hpp"

SERVICE_STATUS g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;
constexpr char SERVICE_NAME[] = "WinAgent";

VOID WINAPI WinAgentService(DWORD argc, LPTSTR *argv);

VOID WINAPI ServiceCtrlHandler(DWORD);

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);


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
    ServiceWorkerThread(LPVOID());
    return 0;
}

DWORD ServiceWorkerThread(LPVOID lpParam) {
    try {
        setup_logger();
    } catch (const std::exception &e) {
        OutputDebugStringA("Failed to load logger");
        return ERROR_LOG_CLIENT_NOT_REGISTERED;
    }
    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0) {
        const std::vector<ProcessInfo> processInfos = loadConfigFile("..\\tests\\output.csv");
        auto bc = BasicComparison(processInfos);
        bc.runDetection();
    }
    return ERROR_SUCCESS;
}


VOID WINAPI WinAgentService(DWORD argc, LPTSTR *argv) {
    const auto statusHandle = RegisterServiceCtrlHandlerA(SERVICE_NAME, ServiceCtrlHandler);
    if (statusHandle == NULL) {
        return;
    }

    g_StatusHandle = statusHandle;

    ZeroMemory(&g_ServiceStatus, sizeof (g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE) {
        OutputDebugStringA("Failed to update service status");
        return;
    }

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        g_ServiceStatus.dwCheckPoint = 1;

        return;
    }

    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE) {
        OutputDebugStringA("Failed to update service status");
        return;
    }

    const HANDLE winApiServiceThreadHandle = CreateThread(
        NULL,
        0,
        ServiceWorkerThread,
        NULL,
        0,
        NULL
    );
    WaitForSingleObject(winApiServiceThreadHandle, INFINITE);

    CloseHandle(g_ServiceStopEvent);

    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 3;

    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE) {
        OutputDebugStringA("Failed to update service status");
    }
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
