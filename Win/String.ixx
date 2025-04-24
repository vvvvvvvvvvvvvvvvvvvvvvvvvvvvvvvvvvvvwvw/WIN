#include "pch.h"

export module String;
import Object;
import <string>;
import <cwchar>;

export namespace win {

    class Char : public Object<Char> {
    private:
        wchar_t ch;
    public:
        Char(char c) : ch(static_cast<unsigned char>(c)) {}
        Char(wchar_t w) : ch(w) {}

        operator char() const { return static_cast<char>(ch); }
        operator wchar_t() const { return ch; }
		operator bool() const { return ch; }
    };

    class String : public Object<String> {
    private:
        std::wstring m_string;
    public:
        using iterator = std::wstring::iterator;
        using const_iterator = std::wstring::const_iterator;

        String() = default;
        String(const char* str);
        String(const wchar_t* str);
        String(const Char* str, size_t size);
        String(const Char* str);
        String(const std::string& str);
        String(const std::wstring& str) : m_string(str) {};
        String(const String& str) = default;
        String(String&& str) noexcept = default;
        String& operator=(const String& str) = default;
        String& operator=(String&& str) noexcept = default;

        static constexpr const char* null = "";

        [[nodiscard]] const wchar_t* data() const noexcept { return m_string.data(); }
        [[nodiscard]] wchar_t* data() noexcept { return m_string.data(); }

        [[nodiscard]] const std::wstring& wstr() const noexcept { return m_string; }
        [[nodiscard]] std::wstring& wstr() noexcept { return m_string; }

        [[nodiscard]] size_t size() const noexcept { return m_string.size(); }
        [[nodiscard]] size_t length() const noexcept { return m_string.length(); }

        bool operator==(const char* str) const;
        bool operator==(const wchar_t* str) const;
        operator std::wstring() const noexcept { return m_string; }

       
        iterator begin() noexcept { return m_string.begin(); }
        iterator end() noexcept { return m_string.end(); }
        const_iterator begin() const noexcept { return m_string.begin(); }
        const_iterator end() const noexcept { return m_string.end(); }
        const_iterator cbegin() const noexcept { return m_string.cbegin(); }
        const_iterator cend() const noexcept { return m_string.cend(); }

        ~String() = default;
    };


    std::ostream& operator<<(std::ostream& os, const String& str);
    std::istream& operator>>(std::istream& is, String& str);
    String operator+(const String& lhs, const String& rhs);
    String operator+(const String& lhs, const char* rhs);
    String operator+(const String& lhs, const wchar_t* rhs);
    String operator+(const char* lhs, const String& rhs);
    String operator+(const wchar_t* lhs, const String& rhs);
    String operator+(const std::string& lhs, const String& rhs);
    String operator+(const String& lhs, const std::string& rhs);
}

export namespace win::cast
{
    std::wstring utf8_to_wstring(const std::string& str);
    std::string wstring_to_utf8(const std::wstring& wstr);
    std::string to_string(const win::Char* str, size_t size);
    std::wstring to_wstring(const win::Char* str, size_t size);
    int to_int(String str);
    std::string to_string(int value);
}