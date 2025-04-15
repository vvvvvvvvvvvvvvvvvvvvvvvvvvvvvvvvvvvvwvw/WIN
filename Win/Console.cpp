#include "pch.h"

module Console;

import Buffer;

namespace win::debug
{
    
    Handle Console::m_output = GetStdHandle(STD_OUTPUT_HANDLE);
    Handle Console::m_input = GetStdHandle(STD_INPUT_HANDLE);

    String Console::ReadLine()
    {
        wchar_t buffer[256]{ 0 };
        DWORD charsRead = 0;
        if (!ReadConsoleW(m_input, buffer, 255, &charsRead, nullptr))
        {
            Hresult hr(GetLastError());
            hr.ThrowIfFailed("Console Read Fail");
        }
        std::wstring input(buffer, charsRead);
        while (!input.empty() && (input.back() == L'\n' || input.back() == L'\r'))
            input.pop_back();
        return String(input);
    }

    void Console::Write(Color color, const String& message)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        WORD originalAttributes = 7;
        if (GetConsoleScreenBufferInfo(m_output, &csbi))
            originalAttributes = csbi.wAttributes;
        SetConsoleTextAttribute(m_output, static_cast<WORD>(color));
        Write(message);
        SetConsoleTextAttribute(m_output, originalAttributes);
    }

    void Console::Write(const String& message)
    {
        WriteConsoleW(m_output, message.data(), message.length(), nullptr, nullptr);
    }

    void Console::Write(const Buffer& buffer)
    {
        WriteConsoleW(m_output, buffer.data(), buffer.len(), nullptr, nullptr);
    }

    void Console::WriteLine(Color color, const String& message)
    {
        Write(color, message);
        Write(L"\n");  
    }

    void Console::WriteLine(const String& message)
    {
        Write(message);
        Write(L"\n");
    }

    void Console::Warning(const String& message)
    {
        WriteLine(Color::Yellow, message);
    }

    void Console::Error(const String& message)
    {
        WriteLine(Color::Red, message);
    }

    void Console::Info(const String& message)
    {
        WriteLine(Color::Gray, message);
    }

    void Console::Success(const String& message)
    {
        WriteLine(Color::Green, message);
    }

    void Console::Clear()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(m_output, &csbi))
            return;
        DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        DWORD count;
        COORD homeCoords = { 0, 0 };
        FillConsoleOutputCharacterW(m_output, L' ', cellCount, homeCoords, &count);
        FillConsoleOutputAttribute(m_output, csbi.wAttributes, cellCount, homeCoords, &count);
        SetConsoleCursorPosition(m_output, homeCoords);
    }

    void Console::SetColor(Color color)
    {
        SetConsoleTextAttribute(m_output, static_cast<WORD>(color));
    }

    void Console::Result(const Hresult& hr, const String& message)
    {
        if (hr.IsSuccess())
        {
            Success(message);
        }
        else
        {
            Error("Error: " + hr.Message());
        }
    }
}
