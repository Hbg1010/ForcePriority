#include "TaskManager.hpp"

HANDLE TaskManager::snapshot = nullptr;


bool TaskManager::setSnapshot() {
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) snapshot = nullptr; // normalize on fail
    return snapshot; // returns if creation is a success
}

/**
 * Finds an app's pid from a string
 * HANDLE can be shared across threads
 * 
 * @param processName: Name of the app you are trying to find
 * 
 * @return DWORD that can either be the ID of the app, or 0. 0 implies something went wrong or the app isn't running / discoverable.
 */
DWORD TaskManager::getProcessIDFromString(const std::wstring& processName) {
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
bool TaskManager::changePriority(DWORD pid, DWORD priorityMode) {
    // improve this a lil eventually
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);

    if (hProcess == NULL) {
        // logging here
        return false;
    }

    bool output = SetPriorityClass(hProcess, priorityMode);
    CloseHandle(hProcess);
    return output;
}

// void TaskManager::closeHandle() {

// }