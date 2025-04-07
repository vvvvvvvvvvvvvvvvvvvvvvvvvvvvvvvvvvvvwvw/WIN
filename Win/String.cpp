#include <string>
#include <iostream>
#include <windows.h>
#include <cstring>

module String;

namespace win
{
    String::String(const char* str)
        : m_string(cast::utf8_to_wstring(str))
    {
    }

    String::String(const wchar_t* str)
        : m_string(str)
    {
    }

    String::String(const std::string& str)
        : m_string(cast::utf8_to_wstring(str))
    {
    }

    String::String(const std::wstring& str)
        : m_string(str)
    {
    }

    String::String(const String& str)
        : m_string(str.m_string)
    {
    }

    String::String(String&& str) noexcept
        : m_string(std::move(str.m_string))
    {
    }

    String& String::operator=(const String& str)
    {
        if (this != &str)
        {
            m_string = str.m_string;
        }
        return *this;
    }

    bool String::operator==(const char* str) const
    {
        return m_string == cast::utf8_to_wstring(str);
    }

    bool String::operator==(const wchar_t* str) const
    {
        return m_string == str;
    }

    String operator+(const char* lhs, const String& rhs)
    {
        std::wstring lhs_wstr = cast::utf8_to_wstring(lhs);
        return String(lhs_wstr + rhs.c_wstr());
    }

    String operator+(const wchar_t* lhs, const String& rhs)
    {
        std::wstring lhs_wstr(lhs);
        return String(lhs_wstr + rhs.c_wstr());
    }

    String operator+(const String& lhs, const char* rhs)
    {
        std::wstring rhs_wstr = cast::utf8_to_wstring(rhs);
        return String(lhs.c_wstr() + rhs_wstr);
    }

    String operator+(const String& lhs, const wchar_t* rhs)
    {
        std::wstring rhs_wstr(rhs);
        return String(lhs.c_wstr() + rhs_wstr);
    }

    String operator+(const String& lhs, const String& rhs)
    {
        return String(lhs.c_wstr() + rhs.c_wstr());
    }

    std::ostream& operator<<(std::ostream& os, const String& str)
    {
        os << win::cast::wstring_to_utf8(str.c_wstr());
        return os;
    }

    std::istream& operator>>(std::istream& is, String& str)
    {
        std::string input;
        is >> input;
        str.GetString() = cast::utf8_to_wstring(input);
        return is;
    }
}

namespace win::cast
{
    // UTF-8 -> UTF-16
    std::wstring utf8_to_wstring(const std::string& str) {
        if (str.empty()) return {};

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
        if (size_needed <= 0)
            return {};
        std::wstring wstr(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);
        return wstr;
    }

    // UTF-16 -> UTF-8
    std::string wstring_to_utf8(const std::wstring& wstr) {
        if (wstr.empty()) return {};

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
        if (size_needed <= 0)
            return {};
        std::string str(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &str[0], size_needed, nullptr, nullptr);
        return str;
    }
}
