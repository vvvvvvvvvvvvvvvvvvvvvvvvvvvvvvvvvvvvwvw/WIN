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

        Process(String name);

        Process(String name, String command);

        void Open(String name, String command);

        void Terminate();

        void Wait();

        Handle Handle() const;

        ~Process();
    };
}
