#include "pch.h"

module String;
import Hresult;

namespace win {

    String::String(const char* str)
        : m_string(cast::utf8_to_wstring(str)) {
    }

    String::String(const wchar_t* str)
        : m_string(str ? str : L"") {
    }

    String::String(const Char* str, size_t size)
        : m_string(cast::to_wstring(str, size)) {
    }

    String::String(const Char* str)
        : m_string([&] {
        std::wstring tmp;
        for (const Char* p = str; p && *p; ++p)
            tmp.push_back(static_cast<wchar_t>(*p));
        return tmp;
            }()) {
    }
    String::String(const std::string& str)
    {
		m_string = cast::utf8_to_wstring(str);
    }
    bool String::operator==(const char* str) const {
        return m_string == cast::utf8_to_wstring(str);
    }
    bool String::operator==(const wchar_t* str) const {
        return m_string == (str ? str : L"");
    }

    std::ostream& operator<<(std::ostream& os, const String& str) {
        os << win::cast::wstring_to_utf8(str.wstr());
        return os;
    }

    std::istream& operator>>(std::istream& is, String& str) {
        std::string input;
        is >> input;
        str.wstr() = cast::utf8_to_wstring(input);
        return is;
    }

    String operator+(const String& lhs, const String& rhs) {
        return String(lhs.wstr() + rhs.wstr());
    }
    String operator+(const String& lhs, const char* rhs) {
        return String(lhs.wstr() + cast::utf8_to_wstring(rhs));
    }
    String operator+(const String& lhs, const wchar_t* rhs) {
        return String(lhs.wstr() + (rhs ? rhs : L""));
    }
    String operator+(const char* lhs, const String& rhs) {
        return String(cast::utf8_to_wstring(lhs) + rhs.wstr());
    }
    String operator+(const wchar_t* lhs, const String& rhs) {
        return String((lhs ? lhs : L"") + rhs.wstr());
    }
    String operator+(const std::string& lhs, const String& rhs) {
        return String(lhs) + rhs;
    }
    String operator+(const String& lhs, const std::string& rhs) {
        return lhs + String(rhs);
    }
} 


// ---------------- WIN CAST ----------------
// str to int

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
    std::wstring to_wstring(const Char* str, size_t size)
    {
        std::wstring result;
        for (size_t i = 0; i < size; ++i) {
            result.push_back(str[i]);
        }
        return result;
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
    std::string to_string(const Char* str, size_t size)
    {
        std::string result;
        for (size_t i = 0; i < size; ++i) {
            result.push_back((char)(str[i]));
        }
        return result;
    }

    // INTEGER -> STR
    std::string to_string(int value)
    {
        char* str = nullptr;
        debug::Hresult result = IntToChar(value, str);

        result.ThrowIfFailed("Cast error");
        return str;
    }


    // STR -> INTEGER
    int to_int(String str)
    {
        return StrToInt(str.data());
    }
}
