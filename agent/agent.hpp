#ifndef AGENT_HPP
#define AGENT_HPP

#include <iostream>
#include <windows.h>
#include <vector>
#include <mutex>
#include "commonHeaders/processInfo.hpp"


inline SERVICE_STATUS g_ServiceStatus = {};
inline SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
inline auto g_ServiceStopEvent = INVALID_HANDLE_VALUE;
constexpr char SERVICE_NAME[] = "WinAgent";

inline std::string CONFIG_FILE_PATH = "..\\scripts\\output.csv";
inline std::vector<ProcessInfo> g_ProcessInfos;
inline std::mutex g_ProcessInfosMutex;

VOID WINAPI WinAgentService(DWORD argc, LPTSTR *argv);

VOID WINAPI ServiceCtrlHandler(DWORD);

DWORD WINAPI WinAgentThread(LPVOID lpParam);

HANDLE ConfigFileChangeListener(const char *configFilePath);

long updateProcessList();

void updateServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwCheckPoint);

#endif
