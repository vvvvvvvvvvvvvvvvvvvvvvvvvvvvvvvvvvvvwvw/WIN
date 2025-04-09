#include "pch.h"

module Buffer;

namespace win
{
	Buffer::Buffer(const String& data)
	{
		m_len = data.length();
		m_data = new char[(m_len + 1) * sizeof(wchar_t)];
		memcpy(m_data, data.data(), (m_len + 1) * sizeof(wchar_t));
	}

	Buffer::Buffer(char* data)
	{
		if (data)
		{
			m_len = strlen(data);
			m_data = new char[m_len + 1] {};
			memcpy(m_data, data, m_len);
		}
	}
	Buffer::Buffer(const Buffer& other)
	{
		m_len = other.m_len;
		m_data = new char[m_len + 1] {};
		memcpy(m_data, other.m_data, m_len);
	}
	Buffer& Buffer::operator=(const Buffer& other)
	{
		if (this != &other)
		{
			delete[] m_data;
			m_len = other.m_len;
			m_data = new char[m_len + 1] {};
			memcpy(m_data, other.m_data, m_len);
		}
		return *this;
	}
	Buffer::Buffer(Buffer&& other) noexcept
		: m_data(other.m_data), m_len(other.m_len)
	{
		other.m_data = nullptr;
		other.m_len = 0;
	}
	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		if (this != &other)
		{
			delete[] m_data;
			m_data = other.m_data;
			m_len = other.m_len;
			other.m_data = nullptr;
			other.m_len = 0;
		}
		return *this;
	}
	void Buffer::free()
	{
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
			m_len = 0;
		}
	}
}