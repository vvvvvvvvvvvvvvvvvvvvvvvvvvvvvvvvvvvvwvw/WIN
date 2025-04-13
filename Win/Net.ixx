// Net.ix
#include "pch.h"

export module Net;

import win; // подключаем другие модули по необходимости

// Класс, отвечающий за инициализацию WinSock
class WinSock
{
private:
    static int m_refcount;
    WSADATA m_data; // Используем объект, а не указатель
public:
    WinSock()
    {
        if (++m_refcount == 1)
        {
            // Передаём адрес m_data
            if (WSAStartup(MAKEWORD(2, 2), &m_data))
            {
                throw 500;
            }
        }
    }
    ~WinSock()
    {
        if (--m_refcount == 0)
        {
            WSACleanup();
        }
    }
};

int WinSock::m_refcount{ 0 };

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

       
        void InitializeAddress(SOCKADDR_IN& addr, const String& address, const String& port)
        {
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(cast::to_int(port));
            std::string ip = cast::wstring_to_utf8(address.c_wstr());
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
        }

    public:
        Socket() = default;

        Socket(int domain, int type, int protocol = 0)
            : m_domain(domain), m_type(type), m_protocol(protocol)
        {
            m_handle = socket(m_domain, m_type, m_protocol);
            m_valid = m_handle != INVALID_SOCKET;
        }

        Socket(SOCKET socket, int domain, int type, int protocol)
            : m_handle(socket), m_domain(domain), m_type(type), m_protocol(protocol)
        {
            m_valid = m_handle != INVALID_SOCKET;
        }

       
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

      
        Socket(Socket&& other) noexcept
        {
            *this = std::move(other);
        }

        Socket& operator=(Socket&& other) noexcept
        {
            if (this != &other)
            {
                Close();
                m_handle = other.m_handle;
                m_domain = other.m_domain;
                m_type = other.m_type;
                m_protocol = other.m_protocol;
                m_valid = other.m_valid;
                other.m_handle = INVALID_SOCKET;
                other.m_valid = false;
            }
            return *this;
        }

        bool IsValid() const { return m_valid; }

        void Send(const Buffer& data)
        {
            if (!m_valid)
                throw std::runtime_error("Invalid socket");
            int result = send(m_handle, data.data<char>(), data.len(), 0);
            if (result == SOCKET_ERROR)
                throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
        }

        Buffer Receive(size_t maxSize = 4096)
        {
            if (!m_valid)
                throw std::runtime_error("Invalid socket");
            Buffer buffer(maxSize);
            int received = recv(m_handle, buffer.data<char>(), maxSize, 0);
            if (received <= 0)
                throw std::runtime_error("Receive failed or connection closed: " + std::to_string(WSAGetLastError()));
            buffer.shrink(received);
            return buffer;
        }

        void Bind(const String& address, const String& port)
        {
            if (!m_valid)
                throw std::runtime_error("Invalid socket");

            SOCKADDR_IN addr;
            InitializeAddress(addr, address, port);

            if (bind(m_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
                throw std::runtime_error("Bind failed: " + std::to_string(WSAGetLastError()));
        }

        void Listen(int backlog = SOMAXCONN)
        {
            if (!m_valid)
                throw std::runtime_error("Invalid socket");

            if (listen(m_handle, backlog) == SOCKET_ERROR)
                throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
        }

        void Connect(const String& address, const String& port)
        {
            if (!m_valid)
                throw std::runtime_error("Invalid socket");

            SOCKADDR_IN addr;
            InitializeAddress(addr, address, port);

            if (connect(m_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
                throw std::runtime_error("Connect failed: " + std::to_string(WSAGetLastError()));
        }

        Socket Accept()
        {
            if (!m_valid)
                throw std::runtime_error("Invalid socket");

            sockaddr_in addr;
            int len = sizeof(addr);

            SOCKET client = accept(m_handle, reinterpret_cast<sockaddr*>(&addr), &len);
            if (client == INVALID_SOCKET)
                throw std::runtime_error("Accept failed: " + std::to_string(WSAGetLastError()));

            return Socket(client, addr.sin_family, m_type, m_protocol);
        }

        void Close()
        {
            if (m_handle != INVALID_SOCKET)
            {
                closesocket(m_handle);
                m_handle = INVALID_SOCKET;
                m_valid = false;
            }
        }

        ~Socket() { Close(); }
    };
}
