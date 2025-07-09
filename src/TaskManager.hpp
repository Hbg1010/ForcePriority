#pragma once

#include <tlhelp32.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include "utils.hpp"
// #include <Geode/Geode.hpp>

// static class to manage priorities
class TaskManager {
private:
    static HANDLE snapshot;

public:
    static bool setSnapshot();
    static DWORD getProcessIDFromString(const std::wstring& processName);
    static bool TaskManager::changePriority(DWORD pid, DWORD priorityMode);
    static void closeHandle();
};
