#include "pch.h"

export module String;

import Object;



export namespace win
{

    class Char : public Object<Char>
    {
    private:
        wchar_t ch;
    public:
        Char(char ch);
        Char(wchar_t wch);

        operator char() const
        {
            return static_cast<char>(ch);
        }
        operator wchar_t() const
        {
            return ch;
        }
    };

    //----------------------------------------
    // Char / String Universal types :)
    //----------------------------------------

    class String : public Object<String>
    {
    private:
        std::wstring m_string;
    public:
        String() = default;

        String(const char* str);
        String(const wchar_t* str);
        String(const Char* str, size_t size);
        String(const Char* str);

        String(const std::string& str);
        String(const std::wstring& str);
        String(const String& str);
        String(String&& str) noexcept;

        String& operator=(const String& str);
        String& operator=(String&& str) noexcept = default;

      
        [[nodiscard]] std::wstring c_wstr() const noexcept
        {
            return m_string;
        }
       
        [[nodiscard]] const wchar_t* data() const noexcept
        {
            return m_string.data();
        }

        [[nodiscard]] const wchar_t* data() noexcept
        {
            return m_string.data();
        }

        [[nodiscard]] std::wstring& c_wstr() noexcept
        {
            return m_string;
        }

        [[nodiscard]] size_t length() const
        {
            return m_string.length();
        }

        [[nodiscard]] size_t size() const
        {
            return m_string.size();
        }

        bool operator==(const char* str) const;
        bool operator==(const wchar_t* str) const;

        operator std::wstring() noexcept { return m_string; }
    };

    std::ostream& operator<<(std::ostream& os, const String& str);
    std::istream& operator>>(std::istream& is, String& str);
    String operator+(const String& lhs, const char* rhs);
    String operator+(const String& lhs, const wchar_t* rhs);
    String operator+(const char* lhs, const String& rhs);
    String operator+(const wchar_t* lhs, const String& rhs);
    String operator+(const String& lhs, const String& rhs);
   
}

export namespace win::cast
{
    std::wstring utf8_to_wstring(const std::string& str);
    std::string wstring_to_utf8(const std::wstring& wstr);
    std::string to_string(const Char* str, size_t size);
    std::wstring to_wstring(const Char* str, size_t size);
    int to_int(String str);
}



