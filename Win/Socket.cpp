#include "pch.h"

module Socket;

namespace win::net
{
	void Socket::InitializeAddress(SOCKADDR_IN& addr, const String& address, const String& port)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		//addr.sin_port = htons(cast::to_int(port));
		std::string ip = cast::wstring_to_utf8(address.wstr());
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	Socket::Socket(int domain, int type, int protocol)
		: m_domain(domain), m_type(type), m_protocol(protocol)
	{
		m_handle = socket(m_domain, m_type, m_protocol);
		m_valid = m_handle != INVALID_SOCKET;
	}
	Socket::Socket(SOCKET socket, int domain, int type, int protocol)
		: m_handle(socket), m_domain(domain), m_type(type), m_protocol(protocol)
	{
		m_valid = m_handle != INVALID_SOCKET;
	}
	win::net::Socket::Socket(Socket&& other) noexcept
	{
		*this = std::move(other);
	}
	Socket& Socket::operator=(Socket&& other) noexcept
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
	void Socket::Send(const Buffer& data)
	{
		if (!m_valid)
			throw debug::Error("Invalid socket");
		int result = send(m_handle, data.data<char>(), data.len(), 0);
		if (result == SOCKET_ERROR)
			throw debug::NetError("Cannot send data");
	}
	Buffer Socket::Receive(size_t maxSize)
	{
		if (!m_valid)
			throw debug::Error("Invalid socket");
		Buffer buffer(maxSize);
		int received = recv(m_handle, buffer.data<char>(), maxSize, 0);
		if (received <= 0)
			throw debug::NetError("Receive failed");
		buffer.resize(received);
		return buffer;
	}
	void Socket::Bind(const String& address, const String& port)
	{
		if (!m_valid)
			throw debug::Error("Invalid socket");

		SOCKADDR_IN addr;
		InitializeAddress(addr, address, port);

		if (bind(m_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
			throw debug::NetError("Bind failed");
	}
	void Socket::Listen(int backlog)
	{
		if (!m_valid)
			throw debug::Error("Invalid socket");

		if (listen(m_handle, backlog) == SOCKET_ERROR)
			throw debug::NetError("Listen failed");
	}
	void Socket::Connect(const String& address, const String& port)
	{
		if (!m_valid)
			throw debug::Error("Invalid socket");

		SOCKADDR_IN addr;
		InitializeAddress(addr, address, port);

		if (connect(m_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
			throw debug::NetError("Connect failed");
	}
	Socket Socket::Accept()
	{
		if (!m_valid)
			throw debug::Error("Invalid socket");

		sockaddr_in addr;
		int len = sizeof(addr);

		SOCKET client = accept(m_handle, reinterpret_cast<sockaddr*>(&addr), &len);
		if (client == INVALID_SOCKET)
			throw debug::Error("Accept failed");

		return Socket(client, addr.sin_family, m_type, m_protocol);
	}
	void Socket::Close()
	{
		if (m_handle != INVALID_SOCKET)
		{
			closesocket(m_handle);
			m_handle = INVALID_SOCKET;
			m_valid = false;
		}
	}
}