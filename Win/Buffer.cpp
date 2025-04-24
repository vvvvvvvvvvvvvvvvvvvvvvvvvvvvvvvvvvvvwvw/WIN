#include "pch.h"

module Buffer;

namespace win {

    Buffer::Buffer(const std::wstring& data) {
        m_len = data.length();
        m_capacity = m_len + 1;
        m_data = new wchar_t[m_capacity];
        std::wmemcpy(m_data, data.c_str(), m_capacity);
    }

    Buffer::Buffer(const char* data) {
        if (!data) {
            m_capacity = 1;
            m_data = new wchar_t[1]();
            m_len = 0;
        }
        else {
            size_t mb_len = std::strlen(data);
            m_capacity = mb_len + 1;
            m_data = new wchar_t[m_capacity];
            std::mbstowcs(m_data, data, m_capacity);
            m_len = std::wcslen(m_data);
        }
    }

    Buffer::Buffer(size_t size) {
        m_capacity = (size > 0 ? size : 1);
        m_data = new wchar_t[m_capacity]();
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
        : m_data(other.m_data), m_len(other.m_len), m_capacity(other.m_capacity)
    {
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
        delete[] m_data;
    }

    void Buffer::resize(size_t newSize) {
        if (newSize == 0) newSize = 1;            
        if (newSize == m_capacity) return;

        wchar_t* temp = new wchar_t[newSize]();
        size_t toCopy = (m_len < newSize - 1 ? m_len : newSize - 1);
        if (m_data) {
            std::wmemcpy(temp, m_data, toCopy);
        }
        temp[toCopy] = L'\0';

        delete[] m_data;
        m_data = temp;
        m_capacity = newSize;
        m_len = toCopy;
    }

    void Buffer::free() {
        delete[] m_data;
        m_data = nullptr;
        m_len = 0;
        m_capacity = 0;
    }

} 