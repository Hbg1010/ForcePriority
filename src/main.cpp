#include <Geode/Geode.hpp>
#include "utils.hpp"
using namespace geode::prelude;

static HANDLE snapshot;

/**
 * Finds an app's pid from a string
 * HANDLE can be shared across threads
 * 
 * @param processName: Name of the app you are trying to find
 * 
 * @return DWORD that can either be the ID of the app, or 0. 0 implies something went wrong or the app isn't running / discoverable.
 */
DWORD getProcessIDFromString(const std::wstring& processName) {
    // to be returned
    DWORD pid = 0;
    // HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
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

    return pid;
}

/**
 * Changes the priority of an exe based off of its pid
 * Must create a HANDLE to do this base off the pid
 * 
 * @param pid: the id of the app
 * @param priorityMode: Specified priority mode. ranges from low to realtime
 * 
 * @return success of the program running
 */
bool changePriority(DWORD pid, DWORD priorityMode) { 
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);

    if (hProcess == NULL) {
        CloseHandle(hProcess);
        log::debug(":(");
        return false;
    }

    bool output = SetPriorityClass(hProcess, priorityMode);
    CloseHandle(hProcess);
    return output;
}

// used to boot the program
$on_mod(Loaded) {
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return;

    log::debug("sucessful load!");
    
    // turns string to w string so i remember later :)
    //     const std::wstring wProcessName(processName.begin(), processName.end());
    std::string process = "discord.exe";
    task_details FirstTask(&process, BELOW_NORMAL_PRIORITY_CLASS);
    DWORD pid = getProcessIDFromString(FirstTask.process);

    log::debug("{}", pid);

    if (pid) {
        if (changePriority(pid, FirstTask.priority)) log::debug("what the skibidi");
    }
}