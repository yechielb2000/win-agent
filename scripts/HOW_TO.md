## How to run the service.

```shell
sc create WinAgent binPath= "path\to\WinAgent.exe"
```

> Make sure an `win_agent_config_file.csv` file is in your desktop path before running the service. (yes. disgusting...
> sorry)

To run the service run.

```shell
sc start WinAgent
```

Stop with

```shell
sc stop WinAgent
```

##   

## How to debug code.

I used this to determine whether I was in debug mode or not to be able to run my service.
(it simulates exactly what the service does but without the service wrapper).

```cpp
#if DEBUG
    constexpr SERVICE_TABLE_ENTRY ServiceTable[] = {
        {const_cast<LPSTR>(SERVICE_NAME), static_cast<LPSERVICE_MAIN_FUNCTION>(WinAgentService)},
        {NULL, NULL}
    };
    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
        return GetLastError();
    }
#else
    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        return GetLastError();
    }

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

    if (winAgentThreadHandle && fileListenerThreadHandle) {
        const HANDLE handles[] = {fileListenerThreadHandle, winAgentThreadHandle};
        WaitForMultipleObjects(2, handles, TRUE, INFINITE);
    }

    CloseHandle(g_ServiceStopEvent);
    if (winAgentThreadHandle) CloseHandle(winAgentThreadHandle);
    if (fileListenerThreadHandle) CloseHandle(fileListenerThreadHandle);
#endif
```

