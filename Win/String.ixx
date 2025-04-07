#include <iostream>
#include <string>

import Object;

export module String;

export namespace win
{
    class String : public Object<String>
    {
    private:
        std::wstring m_string;
    public:
        String() = default;

        String(const char* str);
        String(const wchar_t* str);

        String(const std::string& str);
        String(const std::wstring& str);
        String(const String& str);
        String(String&& str) noexcept;

        String& operator=(const String& str);
        String& operator=(String&& str) noexcept = default;

        // Возвращает копию внутренней строки
        [[nodiscard]] std::wstring c_wstr() const noexcept
        {
            return m_string;
        }

        // Возвращает указатель на внутренние данные (только для чтения)
        [[nodiscard]] const wchar_t* data() const noexcept
        {
            return m_string.data();
        }

        // Возвращает ссылку на внутреннюю строку для редактирования
        std::wstring& GetString() noexcept
        {
            return m_string;
        }

        bool operator==(const char* str) const;
        bool operator==(const wchar_t* str) const;

        operator std::wstring() noexcept { return m_string; }
    };

    export std::ostream& operator<<(std::ostream& os, const String& str);
    export std::istream& operator>>(std::istream& is, String& str);
    export String operator+(const String& lhs, const char* rhs);
    export String operator+(const String& lhs, const wchar_t* rhs);
    export String operator+(const char* lhs, const String& rhs);
    export String operator+(const wchar_t* lhs, const String& rhs);
    export String operator+(const String& lhs, const String& rhs);
}

export namespace win::cast
{
    std::wstring utf8_to_wstring(const std::string& str);
    std::string wstring_to_utf8(const std::wstring& wstr);
}
