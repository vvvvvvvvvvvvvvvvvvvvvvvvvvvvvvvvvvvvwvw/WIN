#include "pch.h"

export module Process;

import win;
import Threading;


export namespace win::system
{

    class Error : public std::runtime_error
    {
    public:
        Error(int code)
            : std::runtime_error("System Error: " + std::to_string(code)), m_code(code)
        {
        }

        int Code() const { return m_code; }

    private:
        int m_code;
    };

    class Process : public Object<Process>
    {
    private:
        Handle m_hProcess;
        PROCESS_INFORMATION m_pinfo{};
        STARTUPINFO m_sinfo{};

    public:

        Process() = default;

        Process(String name)
        {
            Open(name, String::null);
        }

        Process(String name, String command)
        {
            Open(name, command);
        }

        void Open(String name, String command)
        {
            ZeroMemory(&m_sinfo, sizeof(m_sinfo));
            m_sinfo.cb = sizeof(m_sinfo);

            ZeroMemory(&m_pinfo, sizeof(m_pinfo));

            BOOL result = CreateProcess(
                name.data(),              
                command.data(),              
                nullptr,                       
                nullptr,                       
                FALSE,                         
                0,                           
                nullptr,                    
                nullptr,                      
                &m_sinfo,
                &m_pinfo
            );

            if (!result)
            {
                DWORD error = GetLastError();
                throw win::system::Error(error); 
            }

            m_hProcess = m_pinfo.hProcess;
        }

        void Terminate()
        {
            if (!m_hProcess.IsValid())
                throw 404;

            TerminateProcess(m_hProcess, 0);
        }

        void Wait()
        {
            if (!m_hProcess.IsValid())
                throw 404;

            WaitForSingleObject(m_hProcess, INFINITE);
        }

        Handle Handle() const
        {
            return m_hProcess;
        }

        ~Process()
        {
            if (m_pinfo.hThread)
                CloseHandle(m_pinfo.hThread);
            if (m_pinfo.hProcess)
                CloseHandle(m_pinfo.hProcess);
        }
    };
}
