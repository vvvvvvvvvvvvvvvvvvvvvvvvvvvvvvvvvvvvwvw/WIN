#include "pch.h"

export module Process;

import win;
import Threading;
import Error;


export namespace win::system
{
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
                throw debug::Error("Process Open Error");
            }

            m_hProcess = m_pinfo.hProcess;
        }

        void Terminate()
        {
            if (!m_hProcess.IsValid())
                throw debug::Error("Invalid handle");

            TerminateProcess(m_hProcess, 0);
        }

        void Wait()
        {
            if (!m_hProcess.IsValid())
                throw debug::Error("Invalid handle");

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
