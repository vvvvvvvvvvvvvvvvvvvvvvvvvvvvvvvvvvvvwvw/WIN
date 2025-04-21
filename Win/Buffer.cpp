#include "pch.h"

module Buffer;

namespace win
{
    Buffer::Buffer(const std::wstring& data) {
        m_len = data.length();
        m_capacity = m_len + 1;
        m_data = new wchar_t[m_capacity];
        std::wmemcpy(m_data, data.c_str(), m_capacity);
    }
  
    Buffer::Buffer(const char* data) {
        if (data) {
            size_t len = std::strlen(data);
            m_data = new wchar_t[len + 1];
            std::mbstowcs(m_data, data, len + 1);
            m_len = std::wcslen(m_data);
            m_capacity = m_len + 1;
        }
    }

    Buffer::Buffer(size_t size) {
        m_data = new wchar_t[size]();
        m_capacity = size;
        m_len = 0;
    }
   
    Buffer::Buffer(const Buffer& other) {
        m_len = other.m_len;
        m_capacity = other.m_capacity;
        m_data = new wchar_t[m_capacity];
        std::wmemcpy(m_data, other.m_data, m_capacity);
    }

    Buffer& Buffer::operator=(const Buffer& other) {
        if (this != &other) {
            delete[] m_data;
            m_len = other.m_len;
            m_capacity = other.m_capacity;
            m_data = new wchar_t[m_capacity];
            std::wmemcpy(m_data, other.m_data, m_capacity);
        }
        return *this;
    }

    Buffer::Buffer(Buffer&& other) noexcept
        : m_data(other.m_data), m_len(other.m_len), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_len = 0;
        other.m_capacity = 0;
    }
   
    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_len = other.m_len;
            m_capacity = other.m_capacity;
            other.m_data = nullptr;
            other.m_len = 0;
            other.m_capacity = 0;
        }
        return *this;
    }
    
    Buffer::~Buffer() {
        free();
    }

    void Buffer::resize(size_t size) {
        if (size >= m_capacity) return;

        wchar_t* temp = new wchar_t[size]();
        std::wmemcpy(temp, m_data, size);
        delete[] m_data;
        m_data = temp;
        m_capacity = size;
        m_len = std::wcslen(m_data);
    }

    void Buffer::free() {
        delete[] m_data;
        m_data = nullptr;
        m_len = 0;
        m_capacity = 0;
    }
    size_t Buffer::len() const
    {
        return m_len;
    }
}