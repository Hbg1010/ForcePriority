#pragma once
#include <tlhelp32.h>
#include <windows.h>
#include <tchar.h>

// too lazy to use a struct :sad:
class task_details {
public:
    DWORD priority;
    const std::wstring process;

    task_details(std::wstring m_process, DWORD m_priority)
        : process(m_process), priority(m_priority) {}

    task_details(std::string* m_process, DWORD m_priority)
        : process(m_process->begin(), m_process->end()), priority(m_priority) {}
};

// namespace utils {

// }