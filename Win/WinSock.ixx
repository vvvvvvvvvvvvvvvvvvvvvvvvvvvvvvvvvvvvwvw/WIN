module;
#include "pch.h"

export module WinSock;

import Error;

export class WinSock
{
private:
    static int m_refcount;
    LPWSADATA m_data;
public:
    WinSock()
    {
        if (++m_refcount == 1)
        {
            m_data = new WSAData{};
            if (WSAStartup(MAKEWORD(2, 2), m_data))
            {
                throw win::debug::NetError("Winsock failed");
            }
        }
    }
    ~WinSock()
    {
        if (--m_refcount == 0)
        {
            WSACleanup();
            if (m_data)
            {
                delete m_data;
            }
        }
    }
};

int WinSock::m_refcount{ 0 };
