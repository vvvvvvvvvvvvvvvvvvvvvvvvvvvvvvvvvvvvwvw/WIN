#include "pch.h"

module Time;

namespace win::system
{
	SystemTime Time::GetSystemTime()
    {
        SystemTime st;
        ::GetSystemTime((SYSTEMTIME*)&st);
        return st;
    }
    SystemTime Time::GetLocalTime()
    {
        SystemTime st;
        ::GetLocalTime((SYSTEMTIME*)&st);
        return st;
    }

    l_int PerformanceCounter::GetValue()
    {
        LARGE_INTEGER li;
        ::QueryPerformanceCounter(&li);
        return li;
    }
    l_int PerformanceCounter::GetFrequency()
    {
        LARGE_INTEGER li;
        ::QueryPerformanceFrequency(&li);
        return li;
    }
}