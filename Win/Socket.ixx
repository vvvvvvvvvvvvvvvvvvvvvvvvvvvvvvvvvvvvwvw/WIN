// Net.ix
#include "pch.h"

export module Socket;

import win;
import Error;
import WinSock;

export namespace win::net
{
    
    constexpr int ipv4 = AF_INET;
    constexpr int tcp = SOCK_STREAM;
    constexpr int udp = SOCK_DGRAM;
    constexpr int ipproto_tcp = IPPROTO_TCP;
    constexpr int ipproto_udp = IPPROTO_UDP;

   
    class Socket : public WinSock, public Object<Socket>
    {
    private:
        SOCKET m_handle{ INVALID_SOCKET };
        int m_domain{ 0 };
        int m_type{ 0 };
        int m_protocol{ 0 };
        bool m_valid{ false };

       
        void InitializeAddress(SOCKADDR_IN& addr, const String& address, const String& port);

    public:
        Socket() = default;

        Socket(int domain, int type, int protocol = 0);

        Socket(SOCKET socket, int domain, int type, int protocol);
       
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

      
        Socket(Socket&& other) noexcept;

        Socket& operator=(Socket&& other) noexcept;

        bool IsValid() const { return m_valid; }

        void Send(const Buffer& data);

        Buffer Receive(size_t maxSize = 4096);

        void Bind(const String& address, const String& port);

        void Listen(int backlog = SOMAXCONN);

        void Connect(const String& address, const String& port);

        Socket Accept();

        void Close();

        ~Socket() { Close(); }
    };
}
