#include "pch.h"

export module Error;

import String;

export namespace win::debug
{
    class SystemError : public std::runtime_error
    {
    public:
        SystemError(String message)
            : std::runtime_error(cast::wstring_to_utf8("ERROR [" + win::cast::to_string(GetLastError()) + "]: " + message.c_wstr()))
        {
        }
    
    };
    class Error : public std::runtime_error
    {
    public:
        Error(String message)
            : std::runtime_error(cast::wstring_to_utf8(message.c_wstr()))
        {
        }
    };

    class NetError : public Error
    {
    public:
        NetError(String message) : Error("NETWORK ERROR [" + cast::to_string(WSAGetLastError()) + "]: " + message) {};
    };
}