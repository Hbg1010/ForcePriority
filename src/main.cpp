#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;

// note, could move handle opening to become a routine; get handle, check if valid, then get everything without opening more than 1 handle. just for now keep like this
DWORD getProcessIDFromString(const std::wstring& processName) {
    // to be returned
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &pe)) {
        do {
            if (!_wcsicmp(pe.szExeFile, processName.c_str())) {
                pid = pe.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return pid;
}

$on_mod(Loaded) {
    // turns string to w string so i remember later :)
    //     const std::wstring wProcessName(processName.begin(), processName.end());
    std::string process = "discord.exe";
    task_details FirstTask(&process, BELOW_NORMAL_PRIORITY_CLASS);
    DWORD pid = getProcessIDFromString(FirstTask.process);

    log::debug("{}", pid);

    if (pid) {
        HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);

        if (hProcess == NULL) {
            log::debug(":(");
            return;
        }

        if (!SetPriorityClass(hProcess, FirstTask.priority)) {
            log::debug("tried but failed gang");
        } else {
            log::debug("priority lowered!");
        }

        CloseHandle(hProcess);
    }
}